#pragma once

#include <string>

#include <glannotations/AbstractPrimitiveRenderer.h>

namespace glannotations {
	class DistanceFieldPNGRenderer : public AbstractPrimitiveRenderer {
	public:
		DistanceFieldPNGRenderer(gl::GLuint globalMatricesBindingIndex);
		virtual void draw(const globjects::ref_ptr<glannotations::AbstractAnnotation>& annotation);
	};
}