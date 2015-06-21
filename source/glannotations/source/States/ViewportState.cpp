#include <glannotations/States/ViewportState.h>
#include <glannotations/Renderer/AbstractRenderer.h>
#include <glannotations/AbstractAnnotation.h>
#include <glannotations/States/StateInterpolation.h>

#include <iostream>

glannotations::ViewportState::ViewportState(glm::vec2 ll, glm::vec2 lr, glm::vec2 ur) {
	setExtends(ll, lr, ur);
}

void glannotations::ViewportState::setExtends(glm::vec2 ll, glm::vec2 lr, glm::vec2 ur) {
	setDirty(true);
	m_ll = m_origLL = ll;
	m_lr = m_origLR = lr;
	m_ur = m_origUR = ur;
}

const glm::vec2& glannotations::ViewportState::getLL() const {
	return m_ll;
}

const glm::vec2& glannotations::ViewportState::getUR() const {
	return m_ur;
}

void glannotations::ViewportState::draw(const globjects::ref_ptr<glannotations::AbstractAnnotation>& annotation, const AbstractRenderer& renderer) {
	renderer.drawSetupState(annotation, *this);
}

bool glannotations::ViewportState::isValid() const {
	return	glm::distance(m_ll, m_lr) > 0 &&
			glm::distance(m_lr, m_ur) > 0 &&
			glm::distance(m_ll, m_ur) > 0;
}

const glm::vec2& glannotations::ViewportState::getLR() const {
	return m_lr;
}

globjects::ref_ptr<glannotations::AbstractState> glannotations::ViewportState::interpolateWith(const QuadState& mixState, float mix) {
	return glannotations::Interpolation::interpolate(*this, mixState, mix);
}

globjects::ref_ptr<glannotations::AbstractState> glannotations::ViewportState::interpolateWith(const SplineState& mixState, float mix) {
	return glannotations::Interpolation::interpolate(*this, mixState, mix);
}

globjects::ref_ptr<glannotations::AbstractState> glannotations::ViewportState::interpolateWith(const ViewportState& mixState, float mix) {
	return glannotations::Interpolation::interpolate(*this, mixState, mix);
}

globjects::ref_ptr<glannotations::AbstractState> glannotations::ViewportState::clone() const {
	globjects::ref_ptr<glannotations::AbstractState> clonedState(new ViewportState(m_ll, m_lr, m_ur));
	AbstractState::copyState(*clonedState);
	return clonedState;
}

glannotations::BoundingBox glannotations::ViewportState::getBoundingBox() {
	// will initialize a infinite bounding box -- always visible as intended
	return glannotations::BoundingBox(glm::vec3(std::numeric_limits<float>::lowest()), glm::vec3(std::numeric_limits<float>::max()));
}

void glannotations::ViewportState::updateExtends(glm::vec2 sourceExtends) {
	// reset extends to original ones ...
	// todo: make this prettier (like the ExternalReference system)
	m_ll = m_origLL;
	m_lr = m_origLR;
	m_ur = m_origUR;
	// translate source extends to fit screen space extends
	// e.g. width of a quadratic image on a 16:9 screen space will be multiplied by 9 and height multiplied by 16 to make it independent of aspect ratio
	cropExtends(m_ll, m_lr, m_ur, glm::vec2(sourceExtends.x / getScreenAspectRatio(), sourceExtends.y));
}

float glannotations::ViewportState::getScreenAspectRatio() const {
	return m_screenAspectRatio;
}

void glannotations::ViewportState::setScreenAspectRatio(float val) {
	// do not invalidate if aspect ratio is same as before in order to minimize positioning + sizing overhead
    if (std::abs(m_screenAspectRatio - val) < std::numeric_limits<float>::epsilon()) return;
	setDirty(true);
	m_screenAspectRatio = val;
}
