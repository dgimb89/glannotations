#ifndef GLAT_NVPR_RENDERER_H
#define GLAT_NVPR_RENDERER_H

#include <glat/AbstractRenderer.h>
#include "nvpr_init.h"

namespace glat {
	class NVPRRenderer : public glat::AbstractRenderer {
	public:
		NVPRRenderer();
		virtual void draw(glat::AbstractAnnotation* annotation);
	protected:
		void clearStencilBuffer();
	};
}

#endif // GLAT_NVPR_RENDERER_H
