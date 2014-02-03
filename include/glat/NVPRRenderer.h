#ifndef GLAT_NVPR_RENDERER_H
#define GLAT_NVPR_RENDERER_H

#ifdef OPTION_USE_NVPR
#include <glm/glm.hpp>

#include <glat/AbstractRenderer.h>
#include <glat/Styling.h>
#include "nvpr_init.h"

namespace glat {
	class NVPRRenderer : public glat::AbstractRenderer {
	public:
		NVPRRenderer();
		virtual void draw(glat::AbstractAnnotation* annotation);
	protected:
		void clearStencilBuffer();
		virtual void setupOrthoProjection(glm::vec2 llf, glm::vec2 urb, float width, float height, float yMin = 0.f) const;
		virtual void setupOutline(glat::Styling* outline, float scaleFactor = 1.f);

		bool m_drawOutline = false;
		GLuint m_pathSettings;
	};
}
#endif // OPTION_USE_NVPR

#endif // GLAT_NVPR_RENDERER_H
