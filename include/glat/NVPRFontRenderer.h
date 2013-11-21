#ifndef NV_PATH_RENDERING_H
#define NV_PATH_RENDERING_H

#include <windows.h>
#include <gl/GL.h>

#include <glat/AbstractRenderer.h>

namespace glat{
	class NVPRFontRenderer : public AbstractRenderer {
	public:
		NVPRFontRenderer();
		virtual void render(glat::AbstractAnnotation* annotation);
	protected:
		void initializeFont();
		GLuint m_glyphBase, m_pathTemplate;
		GLfloat yMin, yMax;
		GLfloat underline_position, underline_thickness;
		GLfloat totalAdvance, initialShift;
		GLfloat* xtranslate = NULL;
		int emScale = 2048;
		size_t messageLen;
	};
}

#endif