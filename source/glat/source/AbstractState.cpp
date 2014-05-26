#include <glat/AbstractState.h>

void glat::AbstractState::setStyling(const glow::ref_ptr<glat::Styling>& style) {
	setDirty(true); 
	m_stylings[style->getID()] = style;
}

const glat::Styling* glat::AbstractState::getStyling(std::string ID) const {
	auto itr = m_stylings.find(ID);
	if (itr == m_stylings.end())
		return nullptr;

	return itr->second.get();
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
	setDirty(true);
}
