#ifndef GLAT_ABSTRACT_RENDERER_H
#define GLAT_ABSTRACT_RENDERER_H

#include <glat/Object.h>

namespace glat {

	// forward declaration 
	class AbstractAnnotation;
	class ViewportState;

	class AbstractRenderer : public glat::Object {
		friend class ViewportState;
	public:
		virtual void draw(glat::AbstractAnnotation* annotation) = 0;

	protected:
		virtual void drawSetupState(const glat::ViewportState& state) const = 0;
	};
}

#endif