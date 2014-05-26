#pragma once

#include <glat/NVPRRenderer.h>
#include <glat/Styling.h>

#include <glm/glm.hpp>

namespace glat{
	class FontAnnotation;

	class NVPRFontRenderer : public NVPRRenderer {
	public:
		virtual void draw(glat::AbstractAnnotation* annotation);
	protected:
		virtual void drawSetupState(const glat::ViewportState& state) const override;
		virtual void drawSetupState(const glat::InternalState& state) const override;
		virtual void drawSetupState(const glat::InternalPathState& state) const override;

		void initializeFont(glat::FontAnnotation* annotation);
		void getTextStencelingDimensions(const char* text, const size_t& messageLen, GLfloat* &xtranslate, GLfloat& totalAdvance, GLfloat& yMin, GLfloat& yMax, GLfloat& underline_position, GLfloat& underline_thickness) const;
		void stencilThenCoverText(const size_t& messageLen, const GLfloat* xtranslate) const;
		void drawOutline(const size_t& messageLen, const GLfloat* xtranslate, const glat::Styling* outline) const;
		const char* m_currentText;
		glm::vec4 m_textColor;
		const float emScale = 2048;
		const int numChars = 256;  // ISO/IEC 8859-1 8-bit character range
	};
}