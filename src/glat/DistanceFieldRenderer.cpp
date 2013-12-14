#include <glat/DistanceFieldRenderer.h>
#include <glat/AbstractAnnotation.h>

#include <png.h>
#include <iostream>

const char* path = "./distanceFieldCB.png";
using namespace glat;


void DistanceFieldRenderer::draw(AbstractAnnotation* annotation) {
	if (annotation->isDirty()) {
		char* image = loadDistanceField(path);
		glow::ref_ptr<glow::Texture> texture = createRGBATexture(image);

		m_quad = new glat::Quad(texture);
		
		annotation->setDirty(false);
	}

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	m_quad->draw();
	glDisable(GL_BLEND);
}

void glat::DistanceFieldRenderer::drawSetupState(const glat::ViewportState& state) const {

}


glow::ref_ptr<glow::Texture> DistanceFieldRenderer::createRGBATexture(const char* image) {
	glow::ref_ptr<glow::Texture> texture = new glow::Texture(GL_TEXTURE_2D);
	texture->setParameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	texture->setParameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	texture->setParameter(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	texture->setParameter(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	texture->setParameter(GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	texture->image2D(0, GL_RGBA8, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	return texture;
}


char* DistanceFieldRenderer::loadDistanceField(const char* path) {
	char header[8];	// 8 is the maximum size that can be checked

	/* open file */
	FILE *file = fopen(path, "rb");
	if (!file) {
		std::cerr << "File " << path << " could not be opened for reading";
	}

	fread(header, 1, 8, file);

	/* initialize stuff */
	png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if (!png_ptr) {
		std::cerr << "File " << path << ": creating readstruct failed";
	}

	png_infop info_ptr = png_create_info_struct(png_ptr);
	png_infop end_info_ptr = png_create_info_struct(png_ptr);
	if (!info_ptr || !end_info_ptr) {
		std::cerr << "File " << path << ": creating infostruct failed";
	}

	if (setjmp(png_jmpbuf(png_ptr))) {
		std::cerr << "File " << path << ": Error during init_io";
	}
		 
	png_init_io(png_ptr, file);
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

	char* data = new char[m_width * m_height * 4];

	for (int y = 0; y < height; y++)
	{
		png_read_rows(png_ptr, (png_bytepp)&row_pointer, NULL, 1);
		for (int x = 0; x < this->m_width; x++) {
			png_bytep col = &row_pointer[x * 4];
			int idx = 4 * (m_width * (m_height - 1 - y) + x);
			data[idx] = static_cast<char>(col[0]);
			data[idx + 1] = static_cast<char>(col[1]);
			data[idx + 2] = static_cast<char>(col[2]);
			data[idx + 3] = static_cast<char>(col[3]);
		}
	}
	png_read_end(png_ptr, end_info_ptr);

	png_destroy_read_struct(&png_ptr, &info_ptr, &end_info_ptr);

	fclose(file);
	return data;
}