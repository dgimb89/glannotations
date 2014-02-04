#include <glat/preprocessor/GlyphSetGenerator.h>
#include <glat/GlyphSetConfig.h>
#include "config.h"

#include <iostream>
#include <ft2build.h>
#include FT_FREETYPE_H

#define PT_SIZE 196
#define GLYPHSET_BEGIN 33
#define SCALEDOWN_HEIGHT 128
#define GLYPH_GROUP_SIZE 16

inline void handleError(const FT_Error& ftError) {
	if (!ftError) return;
	std::cerr << "Error: " << ftError << " in " << __FILE__ << ":"<< __LINE__ << std::endl;
}

void glat::preprocessor::GlyphSetGenerator::generateGlyphset(std::string fontFileName, unsigned numGlyphs, bool overrideExisting /*= false*/) {

	glat::GlyphSetConfig jsonConfig(fontFileName);
	if (!overrideExisting && jsonConfig.getStartGlyph() <= GLYPHSET_BEGIN && jsonConfig.getNumGlyphs() + jsonConfig.getStartGlyph() - GLYPHSET_BEGIN >= numGlyphs) {
		std::cout << "Existing glyphset already includes requested glyphset" << std::endl;
		return;
	}
	FT_Library library;
	handleError(FT_Init_FreeType(&library));

	// note that a face describes a typeface inclusive style -- ‘Times New Roman Regular’ and ‘Times New Roman Italic’ correspond to two different faces
	FT_Face face;
	std::string fontFile(RESOURCES_DIR);
	fontFile += fontFileName;
	handleError(FT_New_Face(library, fontFile.c_str(), 0, &face));

	if (face->charmap == NULL) {
		std::cerr << "Font files without Unicode charmap are not supported yet -- aborting" << std::endl;
		exit(1);
	}

	// check for number of available glyphs
	if (numGlyphs > face->num_glyphs) {
		std::cerr << "Font face does not support " << numGlyphs << " glyphs (setting to available number: " << face->num_glyphs << ")" << std::endl;
		numGlyphs = face->num_glyphs;
	}

	std::vector<glow::ref_ptr<glat::PNGImage>> glyphImages;
	size_t rowWidth = 0, maxRowWidth = 0;

	handleError(FT_Set_Char_Size(	face,		/* handle to face object */ 
									0,			/* char_weight same as height */ 
									PT_SIZE * 64,	/* char_height in 1/64th of points */
									300,		/* horizontal device resolution */ 
									300)		/* vertical device resolution */);
	for (FT_ULong i = 0; i < numGlyphs; ++i) {
		FT_GlyphSlot slot = face->glyph; // just a shortcut for the current to-be-rendered glyph
		// we begin with glyph mapped to ascii 33 -- everything before that is whitespace anyway
		handleError(FT_Load_Char(face, FT_ULong(GLYPHSET_BEGIN + i), FT_LOAD_RENDER)); // automatically rendered to AA 8bit grayscale bitmap
		glow::ref_ptr<glat::PNGImage> glyphImage = generateGlyphImage(&slot->bitmap, std::abs(slot->bitmap_left), convertFontToPixelSize(face->size->metrics.ascender), convertFontToPixelSize(face->size->metrics.descender), convertFontToPixelSize(slot->metrics.horiBearingY));
		glyphImage->distanceTransform();
		glyphImage->scaleToHeight(SCALEDOWN_HEIGHT);
		rowWidth += glyphImage->getWidth();

		if ((i + 1) % GLYPH_GROUP_SIZE == 0) {
			if (rowWidth > maxRowWidth) maxRowWidth = rowWidth;
			rowWidth = 0;
		}

		glyphImages.push_back(glyphImage);
	}
	if (rowWidth > maxRowWidth) maxRowWidth = rowWidth;

	FT_Done_Face(face);
	FT_Done_FreeType(library);

	// create final glyphset image
	size_t finalHeight = std::ceilf(glyphImages.size() / static_cast<float>(GLYPH_GROUP_SIZE)) * SCALEDOWN_HEIGHT;
	glow::ref_ptr<glat::PNGImage> finalImage = new glat::PNGImage(maxRowWidth, finalHeight, 1);
	size_t width = 0, height = finalImage->getHeight() - SCALEDOWN_HEIGHT;
	unsigned glyphIndex = 0;
	std::vector<glat::GlyphSetConfig::GlyphConfig> glyphConfigs;
	for (auto image : glyphImages) {
		glyphConfigs.push_back(glat::GlyphSetConfig::GlyphConfig(width, height, width + image->getWidth() - 1, height + image->getHeight() - 1));
		for (size_t w = 0; w < image->getWidth(); ++w)
			for (size_t h = 0; h < image->getHeight(); ++h) {
				finalImage->setImageValue(width + w, height + h, 0, image->getImageValue(w, h, 0));
			}
		width += image->getWidth();
		++glyphIndex;
		if (glyphIndex % GLYPH_GROUP_SIZE == 0) {
			width = 0;
			height -= SCALEDOWN_HEIGHT;
		}
	}
	finalImage->saveDistanceField("glyphset.png");

	jsonConfig.setStartGlyph(GLYPHSET_BEGIN);
	jsonConfig.setGlyphConfigs(glyphConfigs, maxRowWidth-1, finalHeight-1);
	jsonConfig.serialize();

}

inline int glat::preprocessor::GlyphSetGenerator::convertFontToPixelSize(int input) {
	return input / 64.0; // 1/64th unit
}

glow::ref_ptr<glat::PNGImage> glat::preprocessor::GlyphSetGenerator::generateGlyphImage(void* bitmap, unsigned marginLeft, int ascender, int descender, int bearingY) {
	FT_Bitmap* bitmapPtr = reinterpret_cast<FT_Bitmap*>(bitmap);
	unsigned imageHeight = ascender - descender;
	unsigned imageWidth = bitmapPtr->width + 2*marginLeft;// +marginRight;
	glow::ref_ptr<glat::PNGImage> result = new glat::PNGImage(imageWidth, imageHeight, 1, 8);

	for (FT_Int w = 0; w < bitmapPtr->width; ++w) {
		for (FT_Int h = 0; h < bitmapPtr->rows; ++h) {
			result->setImageValue(w + marginLeft, imageHeight - ((ascender - bearingY) + h) - 1, 0, 255 - bitmapPtr->buffer[h * bitmapPtr->width + w]);
		}
	}

	return result;
}