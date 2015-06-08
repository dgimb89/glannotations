#pragma once

#include <glm/glm.hpp>

#include <glannotations/Renderer/NVPRRenderer.h>
#include <glannotations/Styles/Styling.h>

namespace glannotations {
	class FontAnnotation;

	class NVPRFontRenderer : public NVPRRenderer {
	public:
		NVPRFontRenderer(gl::GLuint globalMatricesBindingIndex);
		virtual void prepare(const globjects::ref_ptr<glannotations::AbstractAnnotation>& annotation);
		virtual void draw(const globjects::ref_ptr<glannotations::AbstractAnnotation>& annotation);
	protected:
		virtual void drawSetupState(glannotations::ViewportState& state) const override;
		virtual void drawSetupState(glannotations::QuadState& state) const override;
		virtual void drawSetupState(glannotations::SplineState& state) const override;

		void initializeFont(glannotations::FontAnnotation* annotation);
		void getTextStencelingDimensions(const char* text, const size_t& messageLen, gl::GLfloat* &xtranslate, gl::GLfloat& totalAdvance, gl::GLfloat& yMin, gl::GLfloat& yMax, gl::GLfloat& underline_position, gl::GLfloat& underline_thickness) const;
		void stencilThenCoverText(const size_t& messageLen, const gl::GLfloat* xtranslate) const;
		void drawOutline(const size_t& messageLen, const gl::GLfloat* xtranslate, const glannotations::Styling* outline) const;


		const char* m_currentText;
		glm::vec4 m_textColor;
		const float emScale = 2048;
		const int numChars = 256;  // ISO/IEC 8859-1 8-bit character range
	};
}