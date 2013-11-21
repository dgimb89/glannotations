#ifndef ABSTRACT_RENDER_METHOD_H
#define ABSTRACT_RENDER_METHOD_H

#include <glat/AbstractAnnotation.h>

namespace glat {
	class AbstractRenderMethod {
	public:
		virtual void render(glat::AbstractAnnotation annotation) = 0;
	};
}

#endif