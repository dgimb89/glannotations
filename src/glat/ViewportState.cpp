#include <glat/ViewportState.h>
#include <glat/AbstractRenderer.h>
#include <glat/AbstractAnnotation.h>

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

void glat::ViewportState::draw(const AbstractRenderer& renderer) const {
	renderer.drawSetupState(*this);
}

bool glat::ViewportState::isValid() {
	return (m_ur - m_ll).length() > 0;
}

const glm::vec2 glat::ViewportState::getLR() const {
	return glm::vec2(m_ur.x, m_ll.y);
}

void glat::ViewportState::interpolate(const AbstractAnnotation& annotation, AbstractState* secondState, float interpolate) const {
	secondState->interpolate(annotation, *this, interpolate);
}

void glat::ViewportState::interpolate(const AbstractAnnotation& annotation, const ViewportState& viewState, float interpolate) const {
	annotation.interpolate(viewState, *this, interpolate);
}

void glat::ViewportState::interpolate(const AbstractAnnotation& annotation, const InternalState& internalState, float interpolate) const {
	annotation.interpolate(*this, internalState, 1.f - interpolate);
}

void glat::ViewportState::interpolate(const AbstractAnnotation& annotation, const ExternalBoxState& externalState, float interpolate) const {
	annotation.interpolate(*this, externalState, 1.f - interpolate);
}
