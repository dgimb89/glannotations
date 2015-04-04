#pragma once

#include <glannotations/NVPRRenderer.h>

namespace glannotations {
	class SVGAnnotation;
	class NVPRSvgRenderer : public glannotations::NVPRRenderer {
	public:
		NVPRSvgRenderer(gl::GLuint globalMatricesBindingIndex);
		virtual void prepare(const globjects::ref_ptr<glannotations::AbstractAnnotation>& annotation);
		virtual void draw(const globjects::ref_ptr<glannotations::AbstractAnnotation>& annotation) override;
	protected:
		virtual void drawSetupState(const glannotations::ViewportState& state) const override;
		virtual void drawSetupState(const glannotations::InternalState& state) const override;
		virtual void drawSetupState(const glannotations::PathState& state) const override;

		void initializeSVG(const char* pathString);
		void drawPath() const;


		unsigned m_width, m_height;
	};
}
