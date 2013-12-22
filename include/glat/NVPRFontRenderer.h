#ifndef GLAT_NVPR_FONT_RENDERER_H
#define GLAT_NVPR_FONT_RENDERER_H

#include <glat/NVPRRenderer.h>

#include <glm/glm.hpp>

namespace glat{
	class FontAnnotation;

	class NVPRFontRenderer : public NVPRRenderer {
	public:
		virtual void draw(glat::AbstractAnnotation* annotation);
	protected:
		virtual void drawSetupState(const glat::ViewportState& state) const;
		void initializeFont(glat::FontAnnotation* annotation);

		GLuint m_glyphBase;
		const char* m_currentText;
		const float emScale = 2048;
		const int numChars = 256;  // ISO/IEC 8859-1 8-bit character range
	};
}

#endif