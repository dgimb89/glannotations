#pragma once
#include <globjects/base/ref_ptr.h>

#include <glannotations/AbstractPrimitiveRenderer.h>

namespace glannotations {
	class FontAnnotation;

	class DistanceFieldFontRenderer : public glannotations::AbstractPrimitiveRenderer {
	public:
		DistanceFieldFontRenderer(gl::GLuint matricesBindingIndex);
		virtual void prepare(const globjects::ref_ptr<glannotations::AbstractAnnotation>& annotation);
		virtual void draw(const globjects::ref_ptr<glannotations::AbstractAnnotation>& annotation);

	protected:
		void setupGlyphQuadstrip(glannotations::FontAnnotation* annotation);

	};
}
