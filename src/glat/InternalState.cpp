#include <glat/InternalState.h>
#include <glat/AbstractRenderer.h>
#include <glat/AbstractAnnotation.h>

glat::InternalState::InternalState(glm::vec3 ll, glm::vec3 lr, glm::vec3 ur, glowutils::Camera* camera) {
	setExtends(ll, lr, ur);
	setCamera(camera);
	m_interpolation = 0.f;
}

glat::InternalState::InternalState(glm::vec3 ll, glm::vec3 lr, glm::vec3 ur, glowutils::Camera* camera, float interpolate) {
	setExtends(ll, lr, ur);
	setCamera(camera);
	m_interpolation = interpolate;
}

void glat::InternalState::setExtends(glm::vec3 ll, glm::vec3 lr, glm::vec3 ur) {
	setDirty(true);
	m_ll = ll;
	m_lr = lr;
	m_ur = ur;
}

const glm::vec3& glat::InternalState::getLL() const {
	return m_ll;
}

const glm::vec3& glat::InternalState::getLR() const {
	return m_lr;
}


const glm::vec3& glat::InternalState::getUR() const {
	return m_ur;
}

void glat::InternalState::setCamera(glowutils::Camera* camera) {
	setDirty(true);
	m_camera = camera;
}

glowutils::Camera* glat::InternalState::getCamera() const {
	return m_camera;
}

void glat::InternalState::draw(const AbstractRenderer& renderer) const {
	renderer.drawSetupState(*this);
}

bool glat::InternalState::isValid() {
	return (m_ur - m_ll).length() > 0;
}

const glm::mat4& glat::InternalState::getViewProjection() const {
	return m_camProjection = glm::mat4() * m_interpolation + m_camera->viewProjection() * (1.f - m_interpolation);
}

bool glat::InternalState::isDirty() const {
	if (m_camera->viewProjection() != m_camProjection)
		return true;
	return glat::AbstractState::isDirty();
}

void glat::InternalState::interpolate(const AbstractAnnotation& annotation, AbstractState* secondState, float interpolate) const {
	secondState->interpolate(annotation, *this, interpolate);
}

void glat::InternalState::interpolate(const AbstractAnnotation& annotation, const ViewportState& viewState, float interpolate) const {
	annotation.interpolate(viewState, *this, interpolate);
}

void glat::InternalState::interpolate(const AbstractAnnotation& annotation, const InternalState& internalState, float interpolate) const {
	annotation.interpolate(internalState, *this, interpolate);
}
