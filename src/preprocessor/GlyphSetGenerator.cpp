#include <glat/preprocessor/GlyphSetGenerator.h>
#include <iostream>
#include "config.h"

#define PT_SIZE 72

inline void handleError(const FT_Error& ftError) {
	if (!ftError) return;
	std::cerr << "Error: " << ftError << " in " << __FILE__ << ":"<< __LINE__ << std::endl;
}

bool glat::preprocessor::GlyphSetGenerator::generateGlyphset(std::string fontFileName, unsigned numGlyphs, bool extendExisting /*= true*/) {
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

	FT_UInt maxPixelHeight = face->height * PT_SIZE * 300 / 72;

	handleError(FT_Set_Char_Size(	face,		/* handle to face object */ 
									0,			/* char_weight same as height */ 
									PT_SIZE * 64,	/* char_height in 1/64th of points */
									300,		/* horizontal device resolution */ 
									300)		/* vertical device resolution */);
	for (FT_ULong i = 0; i < numGlyphs; ++i) {
		FT_GlyphSlot slot = face->glyph; // just a shortcut for the current to-be-rendered glyph
		// we begin with glyph mapped to ascii 32 (space) -- everything before that is whitespace anyway
		handleError(FT_Load_Char(face, FT_ULong(32 + i), FT_LOAD_RENDER)); // automatically rendered to AA 8bit grayscale bitmap

		// glyph->bitmap is now the expected glyph image
		//slot->bitmap.width
		// create PNGImage here
		// distance transform
	}

	FT_Done_Face(face);
	FT_Done_FreeType(library);

}

glow::ref_ptr<glat::PNGImage> glat::preprocessor::GlyphSetGenerator::generateGlyphImage(FT_Bitmap* bitmap, FT_Int x, FT_Int y) {
	glow::ref_ptr<glat::PNGImage> result = new glat::PNGImage(bitmap->width, bitmap->rows, 1, 8);
	return result;
}
