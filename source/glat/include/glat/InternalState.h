#pragma once

#include <glowutils/Camera.h>
#include <glm/glm.hpp>

#include <glat/AbstractState.h>
#include <glat/glat_api.h>

namespace glat {

	class GLAT_API InternalState : public glat::AbstractState {
	public:
		// lower left, lower right, upper right
		InternalState(glm::vec3 ll, glm::vec3 lr, glm::vec3 ur, glowutils::Camera* camera);
		InternalState(glm::vec3 ll, glm::vec3 lr, glm::vec3 ur, glowutils::Camera* camera, float interpolate);

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

		virtual void interpolate(const AbstractAnnotation& annotation, AbstractState* secondState, float interpolate) const;
		virtual void interpolate(const AbstractAnnotation& annotation, const ViewportState& viewState, float interpolate) const;
		virtual void interpolate(const AbstractAnnotation& annotation, const InternalState& internalState, float interpolate) const;
		virtual void interpolate(const AbstractAnnotation& annotation, const ExternalBoxState& externalState, float interpolate) const;

	protected:
		virtual void draw(const AbstractRenderer& renderer) const;
		glm::vec3 m_ll, m_ur, m_lr;
		glowutils::Camera* m_camera;
		mutable glm::mat4 m_camProjection;
		float m_interpolation;
	};
}
