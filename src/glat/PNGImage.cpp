#include <glat/PNGImage.h>
#include <stdio.h>
#include <png.h>
#include <algorithm>

#include <glat/preprocessor/DistanceFieldGenerator.h>

// internal data wrapper
glat::PNGImage::image_t::image_t(unsigned size) {
	data = new unsigned char[size];
}

glat::PNGImage::image_t::~image_t() {
	delete[] data;
}
// ----------------------

glat::PNGImage::PNGImage(unsigned width, unsigned height, unsigned numComponents /* = 4 */, unsigned bitdepth /* = 8 */) {
	m_width = width;
	m_height = height;
	m_channels = numComponents;
	m_bitdepth = 8;
	createImage();
}

glat::PNGImage::PNGImage(std::string pngFileName) {
	loadImage(pngFileName);
}

glat::PNGImage::PNGImage(std::string pngFileName, std::string destDistanceFieldFile) {
	if (!loadImage(destDistanceFieldFile))
		if (distanceTransformFromPNG(pngFileName))
			saveDistanceField(destDistanceFieldFile);
}

bool glat::PNGImage::distanceTransformFromPNG(std::string pngFileName, unsigned minimalSideLength /* = 500 */) {
	setDirty(true);

	// load source image
	if (!loadImage(pngFileName)) return false;
	glow::ref_ptr<glat::PNGImage> distanceTransform = glat::preprocessor::DistanceFieldGenerator::distanceTransform(*this, minimalSideLength);

	// set new image info data
	m_image = distanceTransform->getImage();
	m_width = distanceTransform->getWidth();
	m_height = distanceTransform->getHeight();
	m_bitdepth = 8;
	m_channels = 1;
	return true;
}

bool glat::PNGImage::distanceTransform(unsigned minimalSideLength /* = 500 */) {
	// load source image
	setDirty(true);
	glow::ref_ptr<glat::PNGImage> distanceTransform = glat::preprocessor::DistanceFieldGenerator::distanceTransform(*this, minimalSideLength);

	// set new image info data
	m_image = distanceTransform->getImage();
	m_width = distanceTransform->getWidth();
	m_height = distanceTransform->getHeight();
	m_channels = 1;
	return true;
}

bool glat::PNGImage::loadImage(std::string pngFileName) {
	char header[8];	// 8 is the maximum size that can be checked

	/* open file */
	FILE* pFile = NULL;
	pFile = fopen(pngFileName.c_str(), "rb");
	if (pFile == NULL) return false;

	fread(header, 1, 8, pFile);
	if (png_sig_cmp(reinterpret_cast<png_const_bytep>(header), 0, 8))
		return false;
	/* initialize stuff */
	png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if (!png_ptr) return false;
	png_infop info_ptr = png_create_info_struct(png_ptr);
	if (!info_ptr) return false;

	if (setjmp(png_jmpbuf(png_ptr))) return false;
	png_init_io(png_ptr, pFile);
	png_set_sig_bytes(png_ptr, 8);
	png_read_info(png_ptr, info_ptr);

	m_width = png_get_image_width(png_ptr, info_ptr);
	m_height = png_get_image_height(png_ptr, info_ptr);
	m_bitdepth = png_get_bit_depth(png_ptr, info_ptr);
	m_channels = png_get_channels(png_ptr, info_ptr);

	switch (png_get_color_type(png_ptr, info_ptr)) {
		case PNG_COLOR_TYPE_PALETTE:
			// convert palette colors to full rgb
			png_set_palette_to_rgb(png_ptr);
			m_channels = 3;
		break;

		case PNG_COLOR_TYPE_GRAY:
			if (m_bitdepth < 8) {
				png_set_expand_gray_1_2_4_to_8(png_ptr);
				m_bitdepth = 8;
			}
		break;
	}

	// if the image has a transperancy set - convert it to a full Alpha channel
	if (png_get_valid(png_ptr, info_ptr, PNG_INFO_tRNS)) {
		png_set_tRNS_to_alpha(png_ptr);
		m_channels += 1;
	}

	// we don't support 16 bit precision yet
	if (m_bitdepth == 16) {
		png_set_strip_16(png_ptr);
		m_bitdepth = 8;
	}

	createImage();

	png_bytep row_pointer = (png_bytep)png_malloc(png_ptr, png_get_rowbytes(png_ptr, info_ptr));
	for (unsigned y = 0; y < m_height; y++) {
		png_read_rows(png_ptr, (png_bytepp)&row_pointer, NULL, 1);
		for (unsigned x = 0; x < this->m_width; x++) {
			// set all components
			for (unsigned numComponent = 0; numComponent < m_channels; ++numComponent) {
				// we do yet only support 8 bit precision; change this when other values are supported
				setImageValue(x, m_height - y - 1, numComponent, static_cast<colorVal_t>(row_pointer[x*m_channels + numComponent]));
			}
		}
	}
	png_free(png_ptr, row_pointer);
	png_destroy_read_struct(&png_ptr, &info_ptr, (png_infopp)0);
	fclose(pFile);
	setDirty(false);
	return true;
}

bool glat::PNGImage::saveDistanceField(std::string pngFileName) const {
	if (m_channels != 1) {
		return false;
	}

	FILE *fp = fopen(pngFileName.c_str(), "wb");
	if (!fp) return false;

	/* initialize stuff */
	png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if (!png_ptr) return false;

	png_infop info_ptr = png_create_info_struct(png_ptr);
	if (!info_ptr) return false;

	if (setjmp(png_jmpbuf(png_ptr))) return false;
	png_init_io(png_ptr, fp);

	/* write header */
	if (setjmp(png_jmpbuf(png_ptr))) return false;

	png_set_IHDR(png_ptr, info_ptr, m_width, m_height,
		sizeof(colorVal_t)*8, PNG_COLOR_TYPE_GRAY, PNG_INTERLACE_NONE,
		PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);

	png_write_info(png_ptr, info_ptr);

	/* write bytes */
	if (setjmp(png_jmpbuf(png_ptr))) return false;
	png_bytepp row_pointers = new png_bytep[m_height];
	for (unsigned h = 0; h < m_height; ++h) {
		row_pointers[h] = reinterpret_cast<png_bytep>(&m_image->data[h * m_width]);
	}
	png_write_image(png_ptr, row_pointers);
	

	/* end write */
	if (setjmp(png_jmpbuf(png_ptr))) return false;
	png_write_end(png_ptr, NULL);

	/* cleanup heap allocation */
	delete[] row_pointers;

	fclose(fp);
	setDirty(false);
	return true;
}

bool glat::PNGImage::isColored(unsigned x, unsigned y) const {
	if (m_channels > 3)
		return getImageValue(x, y, 3) > 0;
	unsigned long result = 0;
	for (auto i = 0; i < m_channels && i < 3; ++i) {
		result += getImageValue(x, y, i);
	}
	return result != 765;
}

void glat::PNGImage::setImageValue(unsigned x, unsigned y, unsigned numComponent, colorVal_t value) {
	imageValue(x, y, numComponent) = value;
}

glat::PNGImage::colorVal_t glat::PNGImage::getImageValue(signed long x, signed long y, signed long numComponent) const {
	// clamp x,y access to image ranges
	x = std::max(0l, std::min(static_cast<signed long>(getWidth() - 1), x));
	y = std::max(0l, std::min(static_cast<signed long>(getHeight() - 1), y));
	return imageValue(x, y, numComponent);
}

glat::PNGImage::colorVal_t& glat::PNGImage::imageValue(unsigned x, unsigned y, unsigned numComponent) const {
	return m_image->data[(m_width * y + x) * m_channels + numComponent];
}

const glow::ref_ptr<glat::PNGImage::image_t> glat::PNGImage::getImage() const {
	return m_image;
}

void glat::PNGImage::createImage() {
	m_image = new image_t(m_height * getRowStride());
}

unsigned glat::PNGImage::getWidth() const {
	return m_width;
}

unsigned glat::PNGImage::getRowStride() const {
	return m_width * m_channels * (m_bitdepth / 8);
}

unsigned glat::PNGImage::getHeight() const {
	return m_height;
}

unsigned glat::PNGImage::getNumComponents() const {
	return m_channels;
}