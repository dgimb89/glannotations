#pragma once

#include <string>

#include <glat/AbstractPrimitiveRenderer.h>

namespace glat {
	class DistanceFieldPNGRenderer : public AbstractPrimitiveRenderer {
	public:
		DistanceFieldPNGRenderer(gl::GLuint globalMatricesBindingIndex);
		virtual void draw(const glow::ref_ptr<glat::AbstractAnnotation>& annotation);
	};
}