#include <glannotations/States/AbstractState.h>
#include <glannotations/Common/globals.h>
#include <glannotations/States/ViewportState.h>
#include <glannotations/States/InternalState.h>
#include <glannotations/States/SplineState.h>

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
	copyTo.setMaximumHeight(getMaximumHeight());
	copyTo.setKeepSourceAspectRatio(getSourceKeepAspectRatio());
}

void glannotations::AbstractState::setStylings(const StylingList& stylings) {
	setDirty(true);
	m_stylings = stylings;
}

const glannotations::StylingList& glannotations::AbstractState::getStylings() const {
	return m_stylings;
}

glannotations::InternalState& glannotations::AbstractState::asInternalState() {
	return dynamic_cast<glannotations::InternalState&>(*this);
}

glannotations::SplineState& glannotations::AbstractState::asSplineState() {
	return dynamic_cast<glannotations::SplineState&>(*this);
}

glannotations::ViewportState& glannotations::AbstractState::asViewportState() {
	return dynamic_cast<glannotations::ViewportState&>(*this);
}

glannotations::Anchor::VerticalAnchor glannotations::AbstractState::getVerticalAnchor() const {
	return m_verticalAnchor;
}

glannotations::Anchor::HorizontalAnchor glannotations::AbstractState::getHorizontalAnchor() const {
	return m_horizontalAnchor;
}

void glannotations::AbstractState::setVerticalAnchor(glannotations::Anchor::VerticalAnchor verticalAnchor) {
	setDirty(true);
	m_verticalAnchor = verticalAnchor;
}

void glannotations::AbstractState::setHorizontalAnchor(glannotations::Anchor::HorizontalAnchor horizontalAnchor) {
	setDirty(true);
	m_horizontalAnchor = horizontalAnchor;
}

void glannotations::AbstractState::setMaximumHeight(float height) {
	setDirty(true);
	m_maximumLineHeight = height;
}

float glannotations::AbstractState::getMaximumHeight() const {
	// important: accomodate for line height in state specialization
	return m_maximumLineHeight;
}

bool glannotations::AbstractState::hasMaximumHeight() const {
	return getMaximumHeight() != 0.f;
}

void glannotations::AbstractState::setKeepSourceAspectRatio(bool keepAspectRatio) {
	setDirty(true);
	m_keepAspectRatio = keepAspectRatio;
}

bool glannotations::AbstractState::getSourceKeepAspectRatio() const {
	return m_keepAspectRatio;
}

void glannotations::AbstractState::prepare() {
	// we deactivate automatic update once prepare is called manually
	if (m_autoUpdate) {
		setAutoUpdate(false);
	}
	// TODO: Interpolation transition management
}

bool glannotations::AbstractState::getAutoUpdate() const {
	return m_autoUpdate;
}

void glannotations::AbstractState::setAutoUpdate(bool val) {
	m_autoUpdate = val;
}