#include <glat/AbstractState.h>

void glat::AbstractState::setStyling(glat::Styling* style) {
	setDirty(true);
	m_stylings[style->getID()] = style;
}

glow::ref_ptr<glat::Styling> glat::AbstractState::getStyling(std::string ID) const {
	auto itr = m_stylings.find(ID);
	if (itr == m_stylings.end()) 
		return nullptr;

	return itr->second;
}

void glat::AbstractState::setAnchor(State::PositionAnchor anchor) {
	setDirty(true);
	m_anchor = anchor;
}

void glat::AbstractState::setAutoExtend(State::AutoExtend extendBehaviour) {
	setDirty(true);
	m_autoExtend = extendBehaviour;
}

glat::AbstractState::AbstractState() {
}
