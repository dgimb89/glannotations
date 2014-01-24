#include <glat/InternalState.h>
#include <glat/AbstractRenderer.h>

glat::InternalState::InternalState(glm::vec3 llf, glm::vec3 urb, glowutils::Camera* camera) {
	setExtends(llf, urb);
	setCamera(camera);
}

void glat::InternalState::setExtends(glm::vec3 llf, glm::vec3 urb) {
	setDirty(true);
	m_llf = llf;
	m_urb = urb;
}

const glm::vec3& glat::InternalState::getLLF() const {
	return m_llf;
}

const glm::vec3& glat::InternalState::getURB() const {
	return m_urb;
}

void glat::InternalState::setCamera(glowutils::Camera* camera) {
	setDirty(true);
	m_camera = camera;
}

const glowutils::Camera* glat::InternalState::getCamera() const {
	return m_camera;
}

void glat::InternalState::draw(const AbstractRenderer& renderer) const {
	renderer.drawSetupState(*this);
}

bool glat::InternalState::isValid() {
	return (m_urb - m_llf).length() > 0;
}
