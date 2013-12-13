#include <glat/AbstractState.h>


void glat::AbstractState::addStyling(glat::Styling* style) {
	m_stylings.push_back(style);
}

const glat::StylingList& glat::AbstractState::getStylings() {
	return m_stylings;
}

bool glat::AbstractState::isValid() {
	return !m_stylings.empty();
}

void glat::AbstractState::setAnchor(State::PositionAnchor anchor) {
	m_anchor = anchor;
}

void glat::AbstractState::setAutoExtend(State::AutoExtend extendBehaviour) {
	m_autoExtend = extendBehaviour;
}