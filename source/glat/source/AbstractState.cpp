#include <glat/AbstractState.h>

#include <glat/ViewportState.h>
#include <glat/InternalState.h>
#include <glat/PathState.h>

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

glat::State::PositionAnchor glat::AbstractState::getAnchor() const {
	return m_anchor;
}

glat::State::AutoExtend glat::AbstractState::getAutoExtend() const {
	return m_autoExtend;
}

void glat::AbstractState::copyState(AbstractState& copyTo) const {
	copyTo.setAutoExtend(getAutoExtend());
	copyTo.setAnchor(getAnchor());
	copyTo.setStylings(getStylings());
}

void glat::AbstractState::setStylings(const StylingList& stylings) {
	m_stylings = stylings;
}

const glat::StylingList& glat::AbstractState::getStylings() const {
	return m_stylings;
}

glat::InternalState& glat::AbstractState::asInternalState() {
	return dynamic_cast<glat::InternalState&>(*this);
}

glat::PathState& glat::AbstractState::asPathState() {
	return dynamic_cast<glat::PathState&>(*this);
}

glat::ViewportState& glat::AbstractState::asViewportState() {
	return dynamic_cast<glat::ViewportState&>(*this);
}
