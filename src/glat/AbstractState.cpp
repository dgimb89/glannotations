#include <glat/AbstractState.h>


void glat::AbstractState::addStyling(glat::Styling* style) {
	m_stylings.push_back(style);
}

const glat::StylingList& glat::AbstractState::getStylings() {
	return m_stylings;
}
