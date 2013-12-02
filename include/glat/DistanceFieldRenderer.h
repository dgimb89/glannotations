#ifndef GLAT_DISTANCE_FIELD_MAPPING_H
#define GLAT_DISTANCE_FIELD_MAPPING_H

#include <windows.h>
#include <GL/gl.h>
#include <GL/glu.h>

#include <glat/AbstractRenderer.h>

namespace glat {
	class DistanceFieldRenderer : public AbstractRenderer {
	public:
		virtual void draw(glat::AbstractAnnotation* annotation);

	private:
		char* loadDistanceField(const char* path);
		GLuint createRGBATexture(const char* image);
		int m_width;
		int m_height;
	};
}

#endif