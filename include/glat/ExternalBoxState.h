#pragma once

#include <glat/AbstractExternalState.h>
#include <glat/AbstractDrawingPrimitive.h>
#include <glm/glm.hpp>

namespace glat {
	class ExternalBoxState : public AbstractExternalState {
	public:
		ExternalBoxState(glm::vec3 llf, glm::vec3 widthSpan, glm::vec3 heightSpan, glm::vec3 depthSpan, glowutils::Camera* camera, bool drawBox = true);
		ExternalBoxState(glm::vec3 llf, glm::vec3 widthSpan, glm::vec3 heightSpan, glm::vec3 depthSpan, glowutils::Camera* camera, float interpolation, bool drawBox = true);

		void setExtends(glm::vec3 llf, glm::vec3 widthSpan, glm::vec3 heightSpan, glm::vec3 depthSpan);

		virtual bool isValid();

		virtual const glm::vec3& getLL() const;
		virtual const glm::vec3& getUR() const;
		virtual const glm::vec3& getLR() const;
		void updateInternalPosition() const;

		virtual const glm::vec3& getLLF() const;
		virtual const glm::vec3& getWidth() const;
		virtual const glm::vec3& getHeight() const;
		virtual const glm::vec3& getDepth() const;

		virtual void interpolate(const AbstractAnnotation& annotation, AbstractState* secondState, float interpolate) const;
		virtual void interpolate(const AbstractAnnotation& annotation, const ViewportState& viewState, float interpolate) const;
		virtual void interpolate(const AbstractAnnotation& annotation, const InternalState& internalState, float interpolate) const;
		virtual void interpolate(const AbstractAnnotation& annotation, const ExternalBoxState& externalState, float interpolate) const;

	protected:
		virtual void draw(const AbstractRenderer& renderer) const;
		inline void updatePositions(glm::vec3 ll, glm::vec3 widthSpan, glm::vec3 heightSpan, glm::vec3 depthSpan, bool backside) const;
		mutable glm::vec3 m_internalLL, m_internalUR, m_internalLR;
		glm::vec3 m_llf, m_widthSpan, m_heightSpan, m_depthSpan;

	};
}