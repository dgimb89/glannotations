#ifndef GLAT_ABSTRACT_RENDERER_H
#define GLAT_ABSTRACT_RENDERER_H

namespace glat {

	// forward declaration 
	class AbstractAnnotation;

	class AbstractRenderer {
	public:
		virtual void draw(glat::AbstractAnnotation* annotation) = 0;
	protected:
		inline void setupViewportRendering();
	};
}

#endif