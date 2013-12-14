#ifndef GLAT_DISTANCE_FIELD_MAPPING_H
#define GLAT_DISTANCE_FIELD_MAPPING_H

#include <glat/Quad.h>
#include <glat/AbstractRenderer.h>

#include <glow/Texture.h>
#include <glow/ref_ptr.h>
#include <glowutils/ScreenAlignedQuad.h>



namespace glat {
	class DistanceFieldRenderer : public AbstractRenderer {
	public:
		virtual void draw(glat::AbstractAnnotation* annotation);

	private:
		char* loadDistanceField(const char* path);
		glow::ref_ptr<glow::Texture> createRGBATexture(const char* image);
		int m_width;
		int m_height;
		glowutils::ScreenAlignedQuad* m_quad;
	};
}

#endif