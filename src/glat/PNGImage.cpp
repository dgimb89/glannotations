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

glat::PNGImage::PNGImage(unsigned width, unsigned height, unsigned numComponents /* = 4 */) {
	m_width = width;
	m_height = height;
	m_imageComponents = numComponents;
	createImage();
}

glat::PNGImage::PNGImage(std::string distanceFieldFile) {
	loadImage(distanceFieldFile);
}

glat::PNGImage::PNGImage(std::string pngSrcFile, std::string destDistanceFieldFile) {
	if (!loadImage(destDistanceFieldFile))
		if (distanceTransformFromPNG(pngSrcFile))
			saveDistanceField(destDistanceFieldFile);
}

bool glat::PNGImage::distanceTransformFromPNG(std::string fileName, unsigned minimalSideLength /* = 30 */) {
	setDirty(true);

	// load source image
	if(!loadImage(fileName)) return false;
	glow::ref_ptr<glat::PNGImage> distanceTransform = glat::preprocessor::DistanceFieldGenerator::distanceTransform(*this);

	// set new image info data
	m_image = distanceTransform->getImage();
	m_width = distanceTransform->getWidth();
	m_height = distanceTransform->getHeight();
	m_imageComponents = 1;
	return true;
}

bool glat::PNGImage::loadImage(std::string fileName) {
	char header[8];	// 8 is the maximum size that can be checked

	/* open file */
	FILE* pFile = NULL;
	pFile = fopen(fileName.c_str(), "rb");
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
	png_byte colorType = png_get_color_type(png_ptr, info_ptr);
	switch (colorType) {
	case PNG_COLOR_TYPE_RGB:
		// handle background color as transparent
		m_imageComponents = 3;
		break;

	case PNG_COLOR_TYPE_RGBA:
		// trivial case here
		m_imageComponents = 4;
		break;

	case PNG_COLOR_TYPE_GRAY:
		// typical distance field format
		m_imageComponents = 1;
		break;

	default:
		// we don't support other color types - break it
		fclose(pFile);
		return false;
	}

	png_bytep row_pointer = (png_bytep)png_malloc(png_ptr, png_get_rowbytes(png_ptr, info_ptr));

	createImage();

	for (unsigned y = 0; y < m_height; y++) {
		png_read_rows(png_ptr, (png_bytepp)&row_pointer, NULL, 1);
		for (unsigned x = 0; x < this->m_width; x++) {
			// set all components
			for (unsigned numComponent = 0; numComponent < m_imageComponents; ++numComponent) {
				setImageValue(x, y, numComponent, static_cast<colorVal_t>(row_pointer[x*m_imageComponents + numComponent]));
			}
		}
	}

	fclose(pFile);
	setDirty(false);
	return true;
}

bool glat::PNGImage::saveDistanceField(std::string fileName) const {
	if (m_imageComponents != 1) {
		return false;
	}

	FILE *fp = fopen(fileName.c_str(), "wb");
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
		colorVal_t* test = m_image->data;
		// we can ignore number of image components as it should be 1 anyway
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
	unsigned result = 0;
	for (unsigned colorChan = 0; colorChan < 3 && colorChan < m_imageComponents; ++colorChan) {
		result += imageValue(x, y, colorChan);
	}

	if (m_imageComponents == 4) 
		result *= imageValue(x, y, 3);

	return result > 0;
}

void glat::PNGImage::setImageValue(unsigned x, unsigned y, unsigned numComponent, colorVal_t value) {
	imageValue(x, y, numComponent) = value;
}

glat::PNGImage::colorVal_t glat::PNGImage::getImageValue(unsigned x, unsigned y, unsigned numComponent /* = 1 */) const {
	return imageValue(x, y, numComponent);
}

glat::PNGImage::colorVal_t& glat::PNGImage::imageValue(unsigned x, unsigned y, unsigned numComponent) const {
	return m_image->data[(m_width * y + x) * m_imageComponents + numComponent];
}

const glow::ref_ptr<glat::PNGImage::image_t> glat::PNGImage::getImage() const {
	return m_image;
}

void glat::PNGImage::createImage() {
	m_image = new image_t(m_height * m_width * m_imageComponents);
}

unsigned glat::PNGImage::getWidth() const {
	return m_width;
}

unsigned glat::PNGImage::getHeight() const {
	return m_height;
}