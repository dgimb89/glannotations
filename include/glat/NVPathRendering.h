#ifndef NV_PATH_RENDERING_H
#define NV_PATH_RENDERING_H

#include <glat/AbstractRenderMethod.h>

namespace glat{
	class NVPathRendering : public AbstractRenderMethod {
	public:
		virtual void render(glat::AbstractAnnotation* annotation);
	};
}

#endif