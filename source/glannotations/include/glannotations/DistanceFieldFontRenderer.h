#pragma once
#include <globjects/base/ref_ptr.h>

#include <glannotations/AbstractPrimitiveRenderer.h>

namespace glat {
	class FontAnnotation;

	class DistanceFieldFontRenderer : public glat::AbstractPrimitiveRenderer {
	public:
		DistanceFieldFontRenderer(gl::GLuint matricesBindingIndex);
		virtual void draw(const globjects::ref_ptr<glat::AbstractAnnotation>& annotation);

	protected:
		void setupGlyphQuadstrip(glat::FontAnnotation* annotation);
	};
}
