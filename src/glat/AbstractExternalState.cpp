#include <glat/AbstractExternalState.h>

glow::ref_ptr<glat::InternalState> glat::AbstractExternalState::getInternalAnnotation() {
	return m_internalState;
}

void glat::AbstractExternalState::setCamera(glowutils::Camera* camera) {
	m_camera = camera;
}

glowutils::Camera* glat::AbstractExternalState::getCamera() const {
	return m_camera;
}

bool glat::AbstractExternalState::isDirty() const {
	if (m_camera->viewProjection() != m_camProjection) {
		return true;
	}
	return glat::AbstractState::isDirty();
}

const glm::mat4& glat::AbstractExternalState::getViewProjection() const {
	return m_camProjection = m_camera->viewProjection();
}

glat::AbstractExternalState::AbstractExternalState(glowutils::Camera* camera) : AbstractState() {
	setCamera(camera);
}
