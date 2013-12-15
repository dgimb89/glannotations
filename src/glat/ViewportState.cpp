#include <glat/ViewportState.h>
#include <glat/AbstractRenderer.h>

glat::ViewportState::ViewportState() {
	m_llf.x = .0f; m_llf.y = .0f;
	m_urb.x = .0f; m_urb.y = .0f;
}

glat::ViewportState::ViewportState(glm::vec2 llf, glm::vec2 urb) {
	setExtends(llf, urb);
}

void glat::ViewportState::setExtends(glm::vec2 llf, glm::vec2 urb) {
	setDirty(true);
	m_llf = llf;
	m_urb = urb;
}

const glm::vec2& glat::ViewportState::getLLF() const {
	return m_llf;
}

const glm::vec2& glat::ViewportState::getURB() const {
	return m_urb;
}

void glat::ViewportState::draw(const AbstractRenderer& renderer) const {
	renderer.drawSetupState(*this);
}

bool glat::ViewportState::isValid() {
	return (m_urb - m_llf).length() > 0;
}
