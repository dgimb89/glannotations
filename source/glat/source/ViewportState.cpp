#include <glat/ViewportState.h>
#include <glat/AbstractRenderer.h>
#include <glat/AbstractAnnotation.h>
#include <glat/StateInterpolation.h>

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

bool glat::ViewportState::isValid() const {
	return (m_ur - m_ll).length() > 0;
}

const glm::vec2 glat::ViewportState::getLR() const {
	return glm::vec2(m_ur.x, m_ll.y);
}

glo::ref_ptr<glat::AbstractState> glat::ViewportState::interpolateWith(const InternalState& mixState, float mix) {
	return glat::Interpolation::interpolate(*this, mixState, mix);
}

glo::ref_ptr<glat::AbstractState> glat::ViewportState::interpolateWith(const PathState& mixState, float mix) {
	return glat::Interpolation::interpolate(*this, mixState, mix);
}

glo::ref_ptr<glat::AbstractState> glat::ViewportState::interpolateWith(const ViewportState& mixState, float mix) {
	return glat::Interpolation::interpolate(*this, mixState, mix);
}

glo::ref_ptr<glat::AbstractState> glat::ViewportState::clone() const {
	glo::ref_ptr<glat::AbstractState> clonedState(new ViewportState(m_ll, m_ur));
	AbstractState::copyState(*clonedState);
	return clonedState;
}

glat::BoundingBox glat::ViewportState::getBoundingBox() {
	return glat::BoundingBox(glm::vec3(std::numeric_limits<float>::lowest()), glm::vec3(std::numeric_limits<float>::max())); // will initialize a infinite bounding box -- always visible as intended
}
