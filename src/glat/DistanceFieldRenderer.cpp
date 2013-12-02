#include <glat/DistanceFieldRenderer.h>
#include <glat/AbstractAnnotation.h>

#include <png.h>
#include <iostream>

const char* path = "./distanceFieldCB.png";
using namespace glat;


void DistanceFieldRenderer::draw(AbstractAnnotation* annotation) {
	if (annotation->isDirty()) {
		char* image = loadDistanceField(path);
		GLuint texture = createRGBATexture(image);
		annotation->setDirty(false);
	}
}


GLuint DistanceFieldRenderer::createRGBATexture(const char* image) {
	GLuint texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	gluBuild2DMipmaps(GL_TEXTURE_2D, 4, m_width, m_height, GL_RGBA, GL_UNSIGNED_BYTE, image);

	return texture;
}


char* DistanceFieldRenderer::loadDistanceField(const char* path) {
	char header[8];	// 8 is the maximum size that can be checked

	/* open file */
	FILE* file = fopen(path, "rb");
	if (!file) std::cerr << "File " << path << " could not be opened for reading";

	fread(header, 1, 8, file);
	/* initialize stuff */
	png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if (!png_ptr) std::cerr << "File " << path << ": creating readstruct failed";

	png_infop info_ptr = png_create_info_struct(png_ptr);
	if (!info_ptr) std::cerr << "File " << path << ": creating infostruct failed";

	if (setjmp(png_jmpbuf(png_ptr))) std::cerr << "File " << path << ": Error during init_io";
		 
	png_init_io(png_ptr, file);
	png_set_sig_bytes(png_ptr, 8);

	png_read_info(png_ptr, info_ptr);

	this->m_width = info_ptr->width;
	this->m_height = info_ptr->height;
	//int color_type = info_ptr->color_type;
	//int bit_depth = info_ptr->bit_depth;

	//number_of_passes = png_set_interlace_handling(png_ptr);
	png_read_update_info(png_ptr, info_ptr);


	/* read file */
	if (setjmp(png_jmpbuf(png_ptr))) std::cerr << "File " << path << ": Error during read_image";

	png_bytep *row_pointers = (png_bytep*)malloc(sizeof(png_bytep)* m_height);
	for (int y = 0; y < m_height; y++)
	{
		row_pointers[y] = (png_byte*)malloc(info_ptr->rowbytes);
	}

	png_read_image(png_ptr, row_pointers);

	// create data pointer
	char* data = new char[m_width * m_height * 4];

	//this->m_pixels[0] = 1.0f;
	//this->m_pixels[3] = 2.0f;

	for (int y = 0; y < m_height; y++) {
		png_byte* row = row_pointers[y];

		for (int x = 0; x < this->m_width; x++) {
			png_byte* col = &(row[x * 4]);
			//printf("Pixel at position (%d, %d) has the Color (%d, %d, %d, %d).\n", x, y, col[0], col[1], col[2], col[3]);
			int idx = 4 * (m_width * y + x);
			data[idx    ] = static_cast<char>(col[0]);
			data[idx + 1] = static_cast<char>(col[1]);
			data[idx + 2] = static_cast<char>(col[2]);
			data[idx + 3] = static_cast<char>(col[3]);
		}
	}

	fclose(file);
	return data;
}