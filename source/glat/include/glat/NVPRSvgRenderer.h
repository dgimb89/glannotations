#pragma once

#include <glat/NVPRRenderer.h>

namespace glat {
	class SVGAnnotation;
	class NVPRSvgRenderer : public glat::NVPRRenderer {
	public:
		virtual void draw(const glow::ref_ptr<glat::AbstractAnnotation>& annotation) override;
	protected:
		virtual void drawSetupState(const glat::ViewportState& state) const override;
		virtual void drawSetupState(const glat::InternalState& state) const override;
		virtual void drawSetupState(const glat::PathState& state) const override;

		void initializeSVG(const char* pathString);
		void drawPath() const;
		unsigned m_width, m_height;
	};
}
