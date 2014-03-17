#ifndef GLAT_ABSTRACT_EXTERNAL_STATE_H
#define GLAT_ABSTRACT_EXTERNAL_STATE_H

#include <glat/AbstractState.h>
#include <glat/InternalState.h>

#include <glowutils/Camera.h>

namespace glat {

	class AbstractExternalState : public glat::AbstractState {

	public:
		AbstractExternalState(glowutils::Camera* camera);

		// TODO: make return state const
		glow::ref_ptr<glat::InternalState> getInternalAnnotation();

		void setCamera(glowutils::Camera* camera);
		glowutils::Camera* getCamera() const;
		const glm::mat4& getViewProjection() const;

		virtual const glm::vec3& getLL() const = 0;
		virtual const glm::vec3& getUR() const = 0;
		virtual const glm::vec3& getLR() const = 0;

		virtual bool isDirty() const;

	protected:
		glow::ref_ptr<glat::InternalState> m_internalState;
		glowutils::Camera* m_camera;
		mutable glm::mat4 m_camProjection;
	};
}

#endif // !GLAT_ABSTRACT_EXTERNAL_STATE_H