#pragma once

#include <glannotations/Renderer/NVPRRenderer.h>

namespace glannotations {
	class SVGAnnotation;
	class NVPRSvgRenderer : public glannotations::NVPRRenderer {
	public:
		NVPRSvgRenderer(gl::GLuint globalMatricesBindingIndex);
		virtual void prepare(const globjects::ref_ptr<glannotations::AbstractAnnotation>& annotation);
		virtual void draw(const globjects::ref_ptr<glannotations::AbstractAnnotation>& annotation) override;
	protected:
		virtual void drawSetupState(const globjects::ref_ptr<glannotations::AbstractAnnotation>& annotation, glannotations::ViewportState& state) const override;
		virtual void drawSetupState(const globjects::ref_ptr<glannotations::AbstractAnnotation>& annotation, glannotations::QuadState& state) const override;
		virtual void drawSetupState(const globjects::ref_ptr<glannotations::AbstractAnnotation>& annotation, glannotations::SplineState& state) const override;

		void initializeSVG(const char* pathString);
		void drawPath() const;


		float m_width, m_height;
	};
}
