#pragma once

#include <glm/glm.hpp>

#include <glat/AbstractState.h>
#include <glat/glat_api.h>

namespace glat {
	class GLAT_API ViewportState : public glat::AbstractState {
	public:
		ViewportState(glm::vec2 llf, glm::vec2 urb);

		// Extends in screen space coordinates
		void setExtends(glm::vec2 llf, glm::vec2 urb);
		const glm::vec2& getLL() const;
		const glm::vec2& getUR() const;
		const glm::vec2 getLR() const;

		virtual bool isValid();

		virtual void interpolate(const AbstractAnnotation& annotation, AbstractState* secondState, float interpolate) const;
		virtual void interpolate(const AbstractAnnotation& annotation, const ViewportState& viewState, float interpolate) const;
		virtual void interpolate(const AbstractAnnotation& annotation, const InternalState& internalState, float interpolate) const;
		virtual void interpolate(const AbstractAnnotation& annotation, const ExternalBoxState& externalState, float interpolate) const;

	protected:
		virtual void draw(const AbstractRenderer& renderer) const;
		glm::vec2 m_ll, m_ur;
	};
}
