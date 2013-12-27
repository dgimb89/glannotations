#include <glat/DistanceFieldImage.h>

glat::DistanceFieldImage::DistanceFieldImage(std::string fileName, unsigned width, unsigned height) {
	loadImage(fileName);
	setWidth(width);
	setHeight(height);
}

glat::DistanceFieldImage::~DistanceFieldImage() {
	delete[] m_image;
}

void glat::DistanceFieldImage::loadImage(std::string fileName) {
	FILE* dfFile;
	fopen(dFile, filename.c_str(), "rt");
}

void glat::DistanceFieldImage::setWidth(unsigned width) {
	m_width = width;
	if(m_height > 0)
		createImage();
}

void glat::DistanceFieldImage::createImage() {
	m_image = new 
}