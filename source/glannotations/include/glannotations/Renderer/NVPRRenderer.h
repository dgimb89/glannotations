#pragma once

#include <glm/glm.hpp>

#include <glannotations/Renderer/AbstractRenderer.h>
#include <glannotations/Styles/Styling.h>

namespace glannotations {
	class QuadState;
	class NVPRRenderer : public glannotations::AbstractRenderer {
	public:
		virtual void draw(const globjects::ref_ptr<glannotations::AbstractAnnotation>& annotation);

	protected:
		NVPRRenderer(gl::GLuint globalMatricesBindingIndex);
		void clearStencilBuffer();
		virtual void setupOrthoProjection(glm::vec2 llf, glm::vec2 urb, float width, float height, float yMin = 0.f) const;
		virtual void setupProjection(glm::mat4 viewProjection) const;
		void setupModelView(glm::mat4 view, const QuadState& state, float stencilWidth, float stencilHeight) const;
		void pushEmptyModelViewMatrix() const;
		void setupOutline(gl::GLuint& pathSettings, const glannotations::Styling* outline, float scaleFactor = 1.f);

		void cleanMatrixStacks() const;

		bool m_drawOutline = false;
		gl::GLuint m_pathBase;
	};
}
