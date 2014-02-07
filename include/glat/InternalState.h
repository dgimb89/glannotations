#ifndef GLAT_INTERNAL_STATE_H
#define GLAT_INTERNAL_STATE_H

#include <glat/AbstractState.h>
#include <glm/glm.hpp>

#include <glowutils/Camera.h>

namespace glat {

	class InternalState : public glat::AbstractState {
	public:
		// lower left, lower right, upper right
		InternalState(glm::vec3 ll, glm::vec3 lr, glm::vec3 ur, glowutils::Camera* camera);

		// Extends in world coordinates
		void setExtends(glm::vec3 ll, glm::vec3 lr, glm::vec3 ur);
		const glm::vec3& getLL() const;
		const glm::vec3& getUR() const;
		const glm::vec3& getLR() const;

		void setCamera(glowutils::Camera* camera);
		const glm::mat4& getViewProjection() const;

		virtual bool isDirty() const;

		virtual bool isValid();

	protected:
		virtual void draw(const AbstractRenderer& renderer) const;
		glm::vec3 m_ll, m_ur, m_lr;
		glowutils::Camera* m_camera;
		mutable glm::mat4 m_camProjection;
	};
}

#endif // GLAT_INTERNAL_STATE_H
