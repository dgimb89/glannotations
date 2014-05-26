#include <glat/ViewportState.h>
#include <glat/AbstractRenderer.h>
#include <glat/AbstractAnnotation.h>
#include <glat/InterpolationManager.h>

glat::ViewportState::ViewportState(glm::vec2 llf, glm::vec2 urb) {
	setExtends(llf, urb);
}

void glat::ViewportState::setExtends(glm::vec2 llf, glm::vec2 urb) {
	setDirty(true);
	m_ll = llf;
	m_ur = urb;
}

const glm::vec2& glat::ViewportState::getLL() const {
	return m_ll;
}

const glm::vec2& glat::ViewportState::getUR() const {
	return m_ur;
}

void glat::ViewportState::draw(const AbstractRenderer& renderer) {
	renderer.drawSetupState(*this);
}

bool glat::ViewportState::isValid() {
	return (m_ur - m_ll).length() > 0;
}

const glm::vec2 glat::ViewportState::getLR() const {
	return glm::vec2(m_ur.x, m_ll.y);
}

glow::ref_ptr<glat::AbstractState> glat::ViewportState::interpolateWith(const InternalState& mixState, float mix) {
	return glat::InterpolationManager::interpolate(*this, mixState, mix);
}

glow::ref_ptr<glat::AbstractState> glat::ViewportState::interpolateWith(const InternalPathState& mixState, float mix) {
	return glat::InterpolationManager::interpolate(*this, mixState, mix);
}

glow::ref_ptr<glat::AbstractState> glat::ViewportState::interpolateWith(const ViewportState& mixState, float mix) {
	return glat::InterpolationManager::interpolate(*this, mixState, mix);
}
