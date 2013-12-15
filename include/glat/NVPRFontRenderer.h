#ifndef GLAT_NV_PATH_RENDERING_H
#define GLAT_NV_PATH_RENDERING_H

#include <glat/AbstractRenderer.h>
#include <glat/Styling.h>

#include <windows.h>
#include <GL/GL.h>
#include <glm/glm.hpp>

namespace glat{
	class FontAnnotation;

	class NVPRFontRenderer : public AbstractRenderer {
	public:
		NVPRFontRenderer();
		virtual void draw(glat::AbstractAnnotation* annotation);
	protected:
		virtual void drawSetupState(const glat::ViewportState& state) const;
		void setupOutline(glat::Styling* outline);
		void initializeFont(glat::FontAnnotation* annotation);
		GLuint m_glyphBase, m_pathTemplate;
		const char* m_currentText;
		bool m_drawOutline = false;

		const float emScale = 2048;
		const int numChars = 256;  // ISO/IEC 8859-1 8-bit character range
	};
}

#endif