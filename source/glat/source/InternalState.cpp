#include <glat/InternalState.h>
#include <glat/AbstractRenderer.h>
#include <glat/AbstractAnnotation.h>
#include <glat/InterpolationManager.h>

glat::InternalState::InternalState(glm::vec3 ll, glm::vec3 lr, glm::vec3 ur, glowutils::Camera* camera) {
	setExtends(ll, lr, ur);
	setCamera(camera);
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

void glat::InternalState::draw(const AbstractRenderer& renderer) {
	if (m_externalReference) {
		m_externalReference->updatePositioning(*this);
		m_externalReference->draw();
	}
	renderer.drawSetupState(*this);
}

bool glat::InternalState::isValid() {
	return (m_ur - m_ll).length() > 0;
}

const glm::mat4& glat::InternalState::getViewProjection() const {
	return m_camProjection = m_camera->viewProjection();	
}

bool glat::InternalState::isDirty() const {
	if (m_camera->viewProjection() != m_camProjection) {
		return true;
	}
	return glat::AbstractState::isDirty();
}

glow::ref_ptr<glat::AbstractState> glat::InternalState::interpolateWith(const InternalState& mixState, float mix) {
	return glat::InterpolationManager::interpolate(*this, mixState, mix);
}

glow::ref_ptr<glat::AbstractState> glat::InternalState::interpolateWith(const InternalPathState& mixState, float mix) {
	return glat::InterpolationManager::interpolate(*this, mixState, mix);
}

glow::ref_ptr<glat::AbstractState> glat::InternalState::interpolateWith(const ViewportState& mixState, float mix) {
	return glat::InterpolationManager::interpolate(*this, mixState, mix);
}
