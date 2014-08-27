#include <glat/AbstractState.h>
#include <glat/globals.h>
#include <glat/ViewportState.h>
#include <glat/InternalState.h>
#include <glat/PathState.h>

void glat::AbstractState::setStyling(const glo::ref_ptr<glat::Styling>& style) {
	setDirty(true); 
	m_stylings[style->getID()] = style;
}

const glat::Styling* glat::AbstractState::getStyling(std::string ID) const {
	auto itr = m_stylings.find(ID);
	if (itr == m_stylings.end())
		return nullptr;

	return itr->second.get();
}

glat::AbstractState::AbstractState() {
	setDirty(true);
}

void glat::AbstractState::copyState(AbstractState& copyTo) const {
	copyTo.setHorizontalAnchor(getHorizontalAnchor());
	copyTo.setVerticalAnchor(getVerticalAnchor());
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

glat::State::VerticalAnchor glat::AbstractState::getVerticalAnchor() const {
	return m_verticalAnchor;
}

glat::State::HorizontalAnchor glat::AbstractState::getHorizontalAnchor() const {
	return m_horizontalAnchor;
}
void glat::AbstractState::setVerticalAnchor(glat::State::VerticalAnchor verticalAnchor) {
	m_verticalAnchor = verticalAnchor;
}

void glat::AbstractState::setHorizontalAnchor(glat::State::HorizontalAnchor horizontalAnchor) {
	m_horizontalAnchor = horizontalAnchor;
}

void glat::AbstractState::setSourceDimensions(unsigned short widthPixel, unsigned short heightPixel, unsigned int bindingIndex) {
	setSourceDimensions(glm::ivec2(widthPixel, heightPixel), bindingIndex);
}

void glat::AbstractState::setSourceDimensions(glm::ivec2 pixelDimensions, unsigned int bindingIndex) {
	m_sourceExtends = glm::vec2(pixelDimensions) / glm::vec2(glat::getViewport(bindingIndex)) * glat::getViewFrustumVolume(bindingIndex);
}
