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
		virtual void drawSetupState(const glat::ViewportState& state) const;
		void initializeFont(glat::FontAnnotation* annotation);
		GLuint m_glyphBase, m_pathTemplate;
		const char* m_currentText;

		const GLfloat emScale = 2048;
		const int numChars = 256;  // ISO/IEC 8859-1 8-bit character range
	};
}

#endif