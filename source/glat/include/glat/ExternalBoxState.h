#pragma once

#include <glm/glm.hpp>

#include <glat/AbstractExternalState.h>
#include <glat/AbstractDrawingPrimitive.h>
#include <glat/glat_api.h>

namespace glat {
	class GLAT_API ExternalBoxState : public AbstractExternalState {
	public:
		ExternalBoxState(glm::vec3 llf, glm::vec3 widthSpan, glm::vec3 heightSpan, glm::vec3 depthSpan, glowutils::Camera* camera, bool drawBox = true);
		ExternalBoxState(glm::vec3 llf, glm::vec3 widthSpan, glm::vec3 heightSpan, glm::vec3 depthSpan, glowutils::Camera* camera, float interpolation, bool drawBox = true);

		void setExtends(glm::vec3 llf, glm::vec3 widthSpan, glm::vec3 heightSpan, glm::vec3 depthSpan);
		virtual bool isValid();
		virtual void updateInternalPosition() const;

		// Todo: make that one smarter ...
		virtual const glm::vec3& getLLF() const;
		virtual const glm::vec3& getWidth() const;
		virtual const glm::vec3& getHeight() const;
		virtual const glm::vec3& getDepth() const;

	protected:
		virtual void draw(const AbstractRenderer& renderer) override;
		inline void updatePositions(glm::vec3 ll, glm::vec3 widthSpan, glm::vec3 heightSpan, glm::vec3 depthSpan, bool backside) const;
		glm::vec3 m_llf, m_widthSpan, m_heightSpan, m_depthSpan;

	};
}