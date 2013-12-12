#ifndef GLAT_NV_PATH_RENDERING_H
#define GLAT_NV_PATH_RENDERING_H

#include <windows.h>
#include <gl/GL.h>

#include <glat/AbstractRenderer.h>

namespace glat{
	class FontAnnotation;

	class NVPRFontRenderer : public AbstractRenderer {
	public:
		NVPRFontRenderer();
		virtual void draw(glat::AbstractAnnotation* annotation);
	protected:
		void initializeFont(glat::FontAnnotation* annotation);
		GLuint m_glyphBase, m_pathTemplate;
		GLfloat yMin, yMax;
		GLfloat underline_position, underline_thickness;
		GLfloat totalAdvance, initialShift;
		GLfloat* xtranslate = NULL;
		const GLfloat emScale = 2048;
	};
}

#endif