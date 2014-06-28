#pragma once

#include <glm/glm.hpp>

#include <glat/NVPRRenderer.h>
#include <glat/Styling.h>

namespace glat {
	class FontAnnotation;

	class NVPRFontRenderer : public NVPRRenderer {
	public:
		NVPRFontRenderer(gl::GLuint globalMatricesBindingIndex);
		virtual void draw(const glow::ref_ptr<glat::AbstractAnnotation>& annotation);
	protected:
		virtual void drawSetupState(const glat::ViewportState& state) const override;
		virtual void drawSetupState(const glat::InternalState& state) const override;
		virtual void drawSetupState(const glat::PathState& state) const override;

		void initializeFont(glat::FontAnnotation* annotation);
		void getTextStencelingDimensions(const char* text, const size_t& messageLen, gl::GLfloat* &xtranslate, gl::GLfloat& totalAdvance, gl::GLfloat& yMin, gl::GLfloat& yMax, gl::GLfloat& underline_position, gl::GLfloat& underline_thickness) const;
		void stencilThenCoverText(const size_t& messageLen, const gl::GLfloat* xtranslate) const;
		void drawOutline(const size_t& messageLen, const gl::GLfloat* xtranslate, const glat::Styling* outline) const;
		const char* m_currentText;
		glm::vec4 m_textColor;
		const float emScale = 2048;
		const int numChars = 256;  // ISO/IEC 8859-1 8-bit character range
	};
}