#pragma once

#include <glat/NVPRRenderer.h>

#include <glm/glm.hpp>

namespace glat{
	class FontAnnotation;

	class NVPRFontRenderer : public NVPRRenderer {
	public:
		virtual void draw(glat::AbstractAnnotation* annotation);
	protected:
		virtual void drawSetupState(const glat::ViewportState& state) const;
		virtual void drawSetupState(const glat::InternalState& state) const;
		virtual void drawSetupState(const glat::ExternalBoxState& state) const;

		void initializeFont(glat::FontAnnotation* annotation);

		GLuint m_glyphBase;
		const char* m_currentText;
		glm::vec4 m_textColor;
		const float emScale = 2048;
		const int numChars = 256;  // ISO/IEC 8859-1 8-bit character range
	};
}