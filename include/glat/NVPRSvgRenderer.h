#ifndef GLAT_NVPR
#define GLAT_NVPR_SVG_RENDERER_H

#include <glat/NVPRRenderer.h>

namespace glat {
	class SVGAnnotation;
	class NVPRSvgRenderer : public glat::NVPRRenderer {
	public:
		virtual void draw(glat::AbstractAnnotation* annotation);
	protected:
		virtual void drawSetupState(const glat::ViewportState& state) const;

		unsigned m_width, m_height;
	};
}

#endif // GLAT_NVPR_SVG_RENDERER_H
