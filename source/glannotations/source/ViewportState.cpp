#include <glannotations/ViewportState.h>
#include <glannotations/AbstractRenderer.h>
#include <glannotations/AbstractAnnotation.h>
#include <glannotations/StateInterpolation.h>

glannotations::ViewportState::ViewportState(glm::vec2 llf, glm::vec2 urb) {
	setExtends(llf, urb);
}

void glannotations::ViewportState::setExtends(glm::vec2 llf, glm::vec2 urb) {
	setDirty(true);
	m_ll = llf;
	m_ur = urb;
}

const glm::vec2& glannotations::ViewportState::getLL() const {
	return m_ll;
}

const glm::vec2& glannotations::ViewportState::getUR() const {
	return m_ur;
}

void glannotations::ViewportState::draw(const AbstractRenderer& renderer) {
	renderer.drawSetupState(*this);
}

bool glannotations::ViewportState::isValid() const {
	return (m_ur - m_ll).length() > 0;
}

const glm::vec2 glannotations::ViewportState::getLR() const {
	return glm::vec2(m_ur.x, m_ll.y);
}

globjects::ref_ptr<glannotations::AbstractState> glannotations::ViewportState::interpolateWith(const InternalState& mixState, float mix) {
	return glannotations::Interpolation::interpolate(*this, mixState, mix);
}

globjects::ref_ptr<glannotations::AbstractState> glannotations::ViewportState::interpolateWith(const PathState& mixState, float mix) {
	return glannotations::Interpolation::interpolate(*this, mixState, mix);
}

globjects::ref_ptr<glannotations::AbstractState> glannotations::ViewportState::interpolateWith(const ViewportState& mixState, float mix) {
	return glannotations::Interpolation::interpolate(*this, mixState, mix);
}

globjects::ref_ptr<glannotations::AbstractState> glannotations::ViewportState::clone() const {
	globjects::ref_ptr<glannotations::AbstractState> clonedState(new ViewportState(m_ll, m_ur));
	AbstractState::copyState(*clonedState);
	return clonedState;
}

glannotations::BoundingBox glannotations::ViewportState::getBoundingBox() {
	return glannotations::BoundingBox(glm::vec3(std::numeric_limits<float>::lowest()), glm::vec3(std::numeric_limits<float>::max())); // will initialize a infinite bounding box -- always visible as intended
}
