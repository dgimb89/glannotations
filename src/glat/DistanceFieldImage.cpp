#include <glat/DistanceFieldImage.h>
#include <stdio.h>
#include <png.h>
#include <algorithm>

glat::DistanceFieldImage::DistanceFieldImage(unsigned width, unsigned height) {
	m_width = width;
	m_height = height;

	createImage();
}

glat::DistanceFieldImage::DistanceFieldImage(std::string distanceFieldFile) {
	loadImage(distanceFieldFile, 1);
}

glat::DistanceFieldImage::DistanceFieldImage(std::string pngSrcFile, std::string destDistanceFieldFile) {
	if (!loadImage(destDistanceFieldFile))
		if (generateFromPNG(pngSrcFile))
			saveImage(destDistanceFieldFile);
}

glat::DistanceFieldImage::~DistanceFieldImage() {
	delete[] m_image;
}

bool glat::DistanceFieldImage::generateFromPNG(std::string fileName, unsigned minimalSideLength /* = 30 */) {
	setDirty(true);

	// load source image
	if(!loadImage(fileName)) return false;
	const int hresW = m_width;
	const int hresH = m_height;

	// calculate new lower res df image
	const int lresW = std::max(minimalSideLength * hresW / hresH, minimalSideLength);
	const int lresH = std::max(minimalSideLength * hresH / hresW, minimalSideLength);
	const int scale = hresW / lresW;
	const unsigned kernelSize = 128;

	DistanceField dfImage = new DistanceFieldValue[lresH * lresW];
	int middleX, middleY;

	for (int actualX = 0; actualX < lresW; actualX++) {
		for (int actualY = 0; actualY < lresH; actualY++) {
			middleX = scale * actualX;
			middleY = scale * actualY;
			bool isBlack = getDistance(middleX, middleY) < 128;	// distance sign
			float distance = sqrtf(powf(kernelSize / 2.0f, 2.f) + powf(kernelSize / 2.0f, 2.f));

			for (int kernelX = std::max(middleX - (int)(kernelSize / 2.0f), 0); kernelX <= std::min(middleX + (int)(kernelSize / 2.0f), hresW - 1); kernelX++) {
				for (int kernelY = std::max(middleY - (int)(kernelSize / 2.0f), 0); kernelY <= std::min(middleY + (int)(kernelSize / 2.0f), hresH - 1); kernelY++) {
					if (isBlack != (getDistance(kernelX, kernelY) < 128)) {
						distance = std::min(distance, std::sqrtf(std::powf(kernelX - middleX, 2.0f) + std::powf(kernelY - middleY, 2.0f)));
					}
				}
			}
			distance /= sqrtf(powf(kernelSize / 2.0f, 2) + powf(kernelSize / 2.0f, 2));
			distance = 1 - distance;
			distance /= 2.0f;
			if (!isBlack)
				distance += 0.3f;
			dfImage[lresW * actualY + actualX] = static_cast<DistanceFieldValue>(std::floor(distance * 255));
		}
	}
	delete[] m_image;
	m_width = lresW;
	m_height = lresH;
	m_image = dfImage;
	return true;
}

bool glat::DistanceFieldImage::loadImage(std::string fileName, unsigned numComponents /* = 4 */) {
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

	png_bytep row_pointer = (png_bytep)png_malloc(png_ptr, png_get_rowbytes(png_ptr, info_ptr));

	if (m_image != nullptr) {
		delete[] m_image;
	}
	createImage();

	for (int y = 0; y < m_height; y++)
	{
		png_read_rows(png_ptr, (png_bytepp)&row_pointer, NULL, 1);
		for (int x = 0; x < this->m_width; x++) {
			setDistance(x, y, static_cast<DistanceFieldValue>(row_pointer[x*numComponents]));
		}
	}

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
		sizeof(DistanceFieldValue)*8, PNG_COLOR_TYPE_GRAY, PNG_INTERLACE_NONE,
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