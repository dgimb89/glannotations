#ifndef GLAT_INTERNAL_STATE_H
#define GLAT_INTERNAL_STATE_H

#include <glat/AbstractState.h>
#include <glm/glm.hpp>

#include <glowutils/Camera.h>

namespace glat {

	class InternalState : public glat::AbstractState {
	public:
		InternalState(glm::vec3 llf, glm::vec3 urb, glowutils::Camera* camera);

		// Extends in screen space coordinates
		void setExtends(glm::vec3 llf, glm::vec3 urb);
		const glm::vec3& getLLF() const;
		const glm::vec3& getURB() const;

		void setCamera(glowutils::Camera* camera);
		const glowutils::Camera* getCamera() const;

		virtual bool isValid();

	protected:
		virtual void draw(const AbstractRenderer& renderer) const;
		glm::vec3 m_llf, m_urb;
		glowutils::Camera* m_camera;
	};
}

#endif // GLAT_INTERNAL_STATE_H
