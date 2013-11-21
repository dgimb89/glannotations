#ifndef ABSTRACT_RENDERER_H
#define ABSTRACT_RENDERER_H

namespace glat {

	// forward declaration 
	class AbstractAnnotation;

	class AbstractRenderer {
	public:
		virtual void render(glat::AbstractAnnotation* annotation) = 0;
	protected:
		inline void setupViewportRendering();
	};
}

#endif