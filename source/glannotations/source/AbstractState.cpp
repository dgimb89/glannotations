#include <glannotations/AbstractState.h>
#include <glannotations/globals.h>
#include <glannotations/ViewportState.h>
#include <glannotations/InternalState.h>
#include <glannotations/PathState.h>

void glannotations::AbstractState::setStyling(const globjects::ref_ptr<glannotations::Styling>& style) {
	setDirty(true); 
	m_stylings[style->getID()] = style;
}

const glannotations::Styling* glannotations::AbstractState::getStyling(std::string ID) const {
	auto itr = m_stylings.find(ID);
	if (itr == m_stylings.end())
		return nullptr;

	return itr->second.get();
}

glannotations::AbstractState::AbstractState() {
	setDirty(true);
}

void glannotations::AbstractState::copyState(AbstractState& copyTo) const {
	copyTo.setHorizontalAnchor(getHorizontalAnchor());
	copyTo.setVerticalAnchor(getVerticalAnchor());
	copyTo.setStylings(getStylings());
}

void glannotations::AbstractState::setStylings(const StylingList& stylings) {
	m_stylings = stylings;
}

const glannotations::StylingList& glannotations::AbstractState::getStylings() const {
	return m_stylings;
}

glannotations::InternalState& glannotations::AbstractState::asInternalState() {
	return dynamic_cast<glannotations::InternalState&>(*this);
}

glannotations::PathState& glannotations::AbstractState::asPathState() {
	return dynamic_cast<glannotations::PathState&>(*this);
}

glannotations::ViewportState& glannotations::AbstractState::asViewportState() {
	return dynamic_cast<glannotations::ViewportState&>(*this);
}

glannotations::State::VerticalAnchor glannotations::AbstractState::getVerticalAnchor() const {
	return m_verticalAnchor;
}

glannotations::State::HorizontalAnchor glannotations::AbstractState::getHorizontalAnchor() const {
	return m_horizontalAnchor;
}
void glannotations::AbstractState::setVerticalAnchor(glannotations::State::VerticalAnchor verticalAnchor) {
	m_verticalAnchor = verticalAnchor;
}

void glannotations::AbstractState::setHorizontalAnchor(glannotations::State::HorizontalAnchor horizontalAnchor) {
	m_horizontalAnchor = horizontalAnchor;
}

void glannotations::AbstractState::setSourceDimensions(unsigned short widthPixel, unsigned short heightPixel, unsigned int bindingIndex) {
	setSourceDimensions(glm::ivec2(widthPixel, heightPixel), bindingIndex);
}

void glannotations::AbstractState::setSourceDimensions(glm::ivec2 pixelDimensions, unsigned int bindingIndex) {
	m_sourceExtends = glm::vec2(pixelDimensions) / glm::vec2(glannotations::getViewport(bindingIndex)) * glannotations::getViewFrustumVolume(bindingIndex);
}
