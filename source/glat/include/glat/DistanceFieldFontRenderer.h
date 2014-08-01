#pragma once
#include <glowbase/ref_ptr.h>

#include <glat/AbstractPrimitiveRenderer.h>

namespace glat {
	class FontAnnotation;

	class DistanceFieldFontRenderer : public glat::AbstractPrimitiveRenderer {
	public:
		DistanceFieldFontRenderer(gl::GLuint matricesBindingIndex);
		virtual void draw(const glow::ref_ptr<glat::AbstractAnnotation>& annotation);

	protected:
		inline void setupGlyphQuadstrip(glat::FontAnnotation* annotation);
	};
}
