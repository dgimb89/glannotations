#pragma once

#include <string>

#include <glat/AbstractPrimitiveRenderer.h>

namespace glat {
	class DistanceFieldPNGRenderer : public AbstractPrimitiveRenderer {
	public:
		virtual void draw(glat::AbstractAnnotation* annotation);
	};
}