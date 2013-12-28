#ifndef GLAT_DISTANCE_FIELD_MAPPING_H
#define GLAT_DISTANCE_FIELD_MAPPING_H

#include <glat/Quad.h>
#include <glat/AbstractRenderer.h>
#include <glat/Styling.h>

#include <glow/Texture.h>
#include <glow/ref_ptr.h>
#include <glowutils/ScreenAlignedQuad.h>

namespace glat {
	class DistanceFieldRenderer : public AbstractRenderer {
	public:
		virtual void draw(glat::AbstractAnnotation* annotation);

	protected:
		virtual void drawSetupState(const glat::ViewportState& state) const;
		glow::ref_ptr<glow::Texture> createRGBATexture(const char* image);
		void setupOutline(glat::Styling* outline);
		void setupBumpMap(glat::Styling* bumpMap);
		int m_width;
		int m_height;
		glat::Quad* m_quad;
	};
}

#endif