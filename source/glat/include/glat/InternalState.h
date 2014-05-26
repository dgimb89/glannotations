#pragma once

#include <glowutils/Camera.h>
#include <glm/glm.hpp>

#include <glat/AbstractExternalReference.h>
#include <glat/AbstractState.h>
#include <glat/glat_api.h>

namespace glat {

	class GLAT_API InternalState : public glat::AbstractState {
	public:
		// lower left, lower right, upper right
		InternalState(glm::vec3 ll, glm::vec3 lr, glm::vec3 ur, glowutils::Camera* camera);

		// Extends in world coordinates
		void setExtends(glm::vec3 ll, glm::vec3 lr, glm::vec3 ur);
		const glm::vec3& getLL() const;
		const glm::vec3& getUR() const;
		const glm::vec3& getLR() const;

		void setCamera(glowutils::Camera* camera);
		glowutils::Camera* getCamera() const;
		const glm::mat4& getViewProjection() const;

		virtual bool isDirty() const;
		virtual bool isValid();

	protected:
		virtual void draw(const AbstractRenderer& renderer) override;

		virtual glow::ref_ptr<AbstractState> interpolateWith(const InternalState& mixState, float mix);
		virtual glow::ref_ptr<AbstractState> interpolateWith(const InternalPathState& mixState, float mix);
		virtual glow::ref_ptr<AbstractState> interpolateWith(const ViewportState& mixState, float mix);

		glm::vec3 m_ll, m_ur, m_lr;
		glowutils::Camera* m_camera;
		mutable glm::mat4 m_camProjection;
		glow::ref_ptr<glat::AbstractExternalReference> m_externalReference = nullptr;
	};
}
