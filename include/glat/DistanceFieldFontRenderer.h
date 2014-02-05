#ifndef GLAT_DISTANCE_FIELD_FONT_RENDERER_H
#define GLAT_DISTANCE_FIELD_FONT_RENDERER_H

#include <string>

#include <glat/Quad.h>
#include <glat/AbstractRenderer.h>
#include <glat/Styling.h>
#include <glat/GlyphSetConfig.h>

#include <glow/Texture.h>
#include <glow/ref_ptr.h>
#include <glowutils/ScreenAlignedQuad.h>

namespace glat {
	class DistanceFieldFontRenderer : public AbstractRenderer {
	public:
		virtual void draw(glat::AbstractAnnotation* annotation);

	protected:
		virtual void drawSetupState(const glat::ViewportState& state) const;
		virtual void drawSetupState(const glat::InternalState& state) const;

		glow::ref_ptr<glow::Texture> createRGBATexture(std::string distanceFieldFile);
		void setupOutline(const glat::Styling* outline);
		void setupBumpMap(const glat::Styling* bumpMap);

		int m_width;
		int m_height;
		glow::ref_ptr<glat::GlyphSetConfig> m_glyphConfig;

		// we set the quad to mutable because there is no outside interference at all - the quad is JUST used for rendering for this specific Renderer
		mutable glow::ref_ptr<glat::Quad> m_quad;
	};
}

#endif