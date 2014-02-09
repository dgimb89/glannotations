#ifndef GLAT_DISTANCE_FIELD_FONT_RENDERER_H
#define GLAT_DISTANCE_FIELD_FONT_RENDERER_H

#include <glat/AbstractPrimitiveRenderer.h>
#include <glat/GlyphSetConfig.h>
#include <glow/ref_ptr.h>

namespace glat {
	class DistanceFieldFontRenderer : public glat::AbstractPrimitiveRenderer {
	public:
		virtual void draw(glat::AbstractAnnotation* annotation);

	protected:
		glow::ref_ptr<glat::GlyphSetConfig> m_glyphConfig;
	};
}

#endif