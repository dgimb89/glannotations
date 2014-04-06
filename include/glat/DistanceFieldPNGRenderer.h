#pragma once

#include <string>

#include <glat/AbstractPrimitiveRenderer.h>
#include <glow/ref_ptr.h>

namespace glat {
	class DistanceFieldPNGRenderer : public AbstractPrimitiveRenderer {
	public:
		virtual void draw(glat::AbstractAnnotation* annotation);
	};
}