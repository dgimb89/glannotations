#ifndef GLAT_DISTANCE_FIELD_PNG_RENDERER_H
#define GLAT_DISTANCE_FIELD_PNG_RENDERER_H

#include <string>

#include <glat/AbstractPrimitiveRenderer.h>
#include <glow/ref_ptr.h>

namespace glat {
	class DistanceFieldPNGRenderer : public AbstractPrimitiveRenderer {
	public:
		virtual void draw(glat::AbstractAnnotation* annotation);
	};
}

#endif // !GLAT_DISTANCE_FIELD_PNG_RENDERER_H