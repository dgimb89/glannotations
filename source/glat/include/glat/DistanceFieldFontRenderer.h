#pragma once
#include <globjects-base/ref_ptr.h>

#include <glat/AbstractPrimitiveRenderer.h>

namespace glat {
	class FontAnnotation;

	class DistanceFieldFontRenderer : public glat::AbstractPrimitiveRenderer {
	public:
		DistanceFieldFontRenderer(gl::GLuint matricesBindingIndex);
		virtual void draw(const glo::ref_ptr<glat::AbstractAnnotation>& annotation);

	protected:
		void setupGlyphQuadstrip(glat::FontAnnotation* annotation);
	};
}
