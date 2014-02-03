#ifndef GLAT_NVPR
#define GLAT_NVPR_SVG_RENDERER_H

#ifdef OPTION_USE_NVPR

#include <glat/NVPRRenderer.h>

namespace glat {
	class SVGAnnotation;
	class NVPRSvgRenderer : public glat::NVPRRenderer {
	public:
		virtual void draw(glat::AbstractAnnotation* annotation);
	protected:
		virtual void drawSetupState(const glat::ViewportState& state) const;
		virtual void drawSetupState(const glat::InternalState& state) const;

		unsigned m_width, m_height;
	};
}
#endif // OPTION_USE_NVPR

#endif // GLAT_NVPR_SVG_RENDERER_H
