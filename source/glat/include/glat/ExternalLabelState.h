#pragma once

#include <glm/glm.hpp>

#include <glat/AbstractExternalState.h>
#include <glat/glat_api.h>

namespace glat {
	class GLAT_API ExternalLabelState : public AbstractExternalState {
	public:
		ExternalLabelState(glm::vec3 reference, glm::vec3 center, float width, float height, glowutils::Camera* camera, bool drawBox = true);
		void setReference(glm::vec3 reference);
		void setExtends(glm::vec3 center, float width, float height);
		virtual void updateInternalPosition() const override;

		virtual bool isValid();

	protected:
		float m_height, m_width;
		glm::vec3 m_reference, m_center;
		virtual void draw(const AbstractRenderer& renderer) override;
	};
}