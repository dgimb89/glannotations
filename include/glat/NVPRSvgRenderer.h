#pragma once

#include <glat/NVPRRenderer.h>

namespace glat {
	class SVGAnnotation;
	class NVPRSvgRenderer : public glat::NVPRRenderer {
	public:
		virtual void draw(glat::AbstractAnnotation* annotation);
	protected:
		virtual void drawSetupState(const glat::ViewportState& state) const;
		virtual void drawSetupState(const glat::InternalState& state) const;
		virtual void drawSetupState(const glat::ExternalBoxState& state) const;
		void initializeSVG(const char* pathString);
		void drawPath() const;
		unsigned m_width, m_height;
	};
}
