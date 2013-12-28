#include <glat/DistanceFieldImage.h>
#include <png.h>

glat::DistanceFieldImage::DistanceFieldImage(unsigned width, unsigned height) {
	m_width = width;
	m_height = height;

	createImage();
}

glat::DistanceFieldImage::DistanceFieldImage(std::string distanceFieldFile) {
	loadImage(distanceFieldFile);
}

glat::DistanceFieldImage::DistanceFieldImage(std::string pngSrcFile, std::string destDistanceFieldFile) {
	if (!loadImage(destDistanceFieldFile))
		if (generateFromPNG(pngSrcFile))
			loadImage(destDistanceFieldFile);
}

glat::DistanceFieldImage::~DistanceFieldImage() {
	delete[] m_image;
}

bool glat::DistanceFieldImage::generateFromPNG(std::string fileName) {
	setDirty(true);
	return true;
}

bool glat::DistanceFieldImage::loadImage(std::string fileName) {
	char header[8];	// 8 is the maximum size that can be checked

	/* open file */
	FILE * pFile;
	pFile = fopen(fileName.c_str(), "rb");
	if (pFile == NULL) return false;

	fread(header, 1, 8, pFile);

	/* initialize stuff */
	png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);

	png_infop info_ptr = png_create_info_struct(png_ptr);
	png_infop end_info_ptr = png_create_info_struct(png_ptr);

	setjmp(png_jmpbuf(png_ptr));
	png_init_io(png_ptr, pFile);
	png_set_sig_bytes(png_ptr, 8);
	png_read_info(png_ptr, info_ptr);

	png_uint_32 width;
	png_uint_32 height;
	int color_type;
	int bit_depth;

	png_get_IHDR(png_ptr, info_ptr, &width, &height, &bit_depth, &color_type, NULL, NULL, NULL);
	m_width = width;
	m_height = height;

	png_bytep row_pointer = (png_bytep)png_malloc(png_ptr, png_get_rowbytes(png_ptr, info_ptr));

	if (m_image != nullptr) {
		delete[] m_image;
	}
	createImage();

	for (int y = 0; y < height; y++)
	{
		png_read_rows(png_ptr, (png_bytepp)&row_pointer, NULL, 1);
		for (int x = 0; x < this->m_width; x++) {
			setDistance(x, y, static_cast<DistanceFieldValue>(row_pointer[x]));
		}
	}
	png_read_end(png_ptr, end_info_ptr);

	png_destroy_read_struct(&png_ptr, &info_ptr, &end_info_ptr);

	fclose(pFile);
	setDirty(false);
	return true;
}

bool glat::DistanceFieldImage::saveImage(std::string fileName) const {
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
		sizeof(DistanceFieldValue), PNG_COLOR_TYPE_GRAY, PNG_INTERLACE_NONE,
		PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);

	png_write_info(png_ptr, info_ptr);

	/* write bytes */
	if (setjmp(png_jmpbuf(png_ptr))) return false;
	png_bytepp row_pointers = new png_bytep[m_height];
	for (unsigned h = 0; h < m_height; ++h) {
		row_pointers[h] = reinterpret_cast<png_bytep>(&m_image[h * m_width]);
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

void glat::DistanceFieldImage::setDistance(unsigned x, unsigned y, DistanceFieldValue distance) {
	m_image[m_width * y + x] = distance;
}

glat::DistanceFieldImage::DistanceFieldValue glat::DistanceFieldImage::getDistance(unsigned x, unsigned y) const {
	return m_image[m_width * y + x];
}

const glat::DistanceFieldImage::DistanceField glat::DistanceFieldImage::getDistanceField() const {
	return m_image;
}

void glat::DistanceFieldImage::createImage() {
	m_image = new DistanceFieldValue[m_height * m_width];
}

unsigned glat::DistanceFieldImage::getWidth() const {
	return m_width;
}

unsigned glat::DistanceFieldImage::getHeight() const {
	return m_height;
}