#pragma once

#include "nvpr_init.h"
#include <glm/glm.hpp>

#include <glat/AbstractRenderer.h>
#include <glat/Styling.h>

namespace glat {
	class NVPRRenderer : public glat::AbstractRenderer {
	public:
		NVPRRenderer();
		virtual void draw(const glow::ref_ptr<glat::AbstractAnnotation>& annotation);

	protected:
		void clearStencilBuffer();
		virtual void setupOrthoProjection(glm::vec2 llf, glm::vec2 urb, float width, float height, float yMin = 0.f) const;
		virtual void setupInternalProjection(glm::mat4 mvp, glm::vec3 ll) const;
		void setupInternalModelview(glm::vec3 ll, glm::vec3 lr, glm::vec3 ur, float stencilWidth, float stencilHeight) const;
		void setupOutline(GLuint& pathSettings, const glat::Styling* outline, float scaleFactor = 1.f);
		bool m_drawOutline = false;
		GLuint m_pathBase;
	};
}
