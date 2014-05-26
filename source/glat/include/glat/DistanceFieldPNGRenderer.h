#pragma once

#include <string>

#include <glat/AbstractPrimitiveRenderer.h>

namespace glat {
	class DistanceFieldPNGRenderer : public AbstractPrimitiveRenderer {
	public:
		virtual void draw(const glow::ref_ptr<glat::AbstractAnnotation>& annotation);
	};
}