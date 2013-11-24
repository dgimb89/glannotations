#ifndef GLAT_ABSTRACT_RENDERER_H
#define GLAT_ABSTRACT_RENDERER_H

#include <glow/Referenced.h>

namespace glat {

	// forward declaration 
	class AbstractAnnotation;

	class AbstractRenderer : glow::Referenced {
	public:
		virtual void draw(glat::AbstractAnnotation* annotation) = 0;
	protected:
		inline void setupViewportRendering();
	};
}

#endif