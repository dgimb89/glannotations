#pragma once
#include <glowbase/ref_ptr.h>

#include <glat/AbstractPrimitiveRenderer.h>
#include <glat/GlyphSetConfig.h>

namespace glat {
	class DistanceFieldFontRenderer : public glat::AbstractPrimitiveRenderer {
	public:
		virtual void draw(const glow::ref_ptr<glat::AbstractAnnotation>& annotation);

	protected:
		glow::ref_ptr<glat::GlyphSetConfig> m_glyphConfig;
	};
}
