#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <glannotations/AbstractAnnotation.h>
#include <glannotations/States/ViewportState.h>
#include <glannotations/States/QuadState.h>
#include <glannotations/States/SplineState.h>
#include <glannotations/Styles/ExternalColor.h>

glannotations::AbstractAnnotation::AbstractAnnotation(const globjects::ref_ptr<glannotations::AbstractState>& state) {
	setState(state);
}

void glannotations::AbstractAnnotation::draw() {
	if (m_enabled == false) return;
	m_renderer->draw(this);
}

globjects::ref_ptr<glannotations::AbstractState> glannotations::AbstractAnnotation::getState() const {
	// todo: implement observer pattern in case of state changes
	m_renderState = nullptr;
	return m_state;
}

void glannotations::AbstractAnnotation::setState(const globjects::ref_ptr<glannotations::AbstractState>& state) {
	m_state = state;
	m_renderState = nullptr;
}

globjects::ref_ptr<glannotations::AbstractState> glannotations::AbstractAnnotation::getRenderState() const {
	setupRenderState();
	return m_renderState;
}

void glannotations::AbstractAnnotation::resetInterpolation() {
	m_renderState = nullptr;
}

void glannotations::AbstractAnnotation::interpolateState(const QuadState& mixState, float mix) {
	m_renderState = nullptr;
	setupRenderState();
	m_renderState = m_state->interpolateWith(mixState, mix);
}

void glannotations::AbstractAnnotation::interpolateState(const SplineState& mixState, float mix) {
	m_renderState = nullptr;
	setupRenderState();
	m_renderState = m_state->interpolateWith(mixState, mix);
}

void glannotations::AbstractAnnotation::interpolateState(const ViewportState& mixState, float mix) {
	m_renderState = nullptr;
	setupRenderState();
	m_renderState = m_state->interpolateWith(mixState, mix);
}

void glannotations::AbstractAnnotation::setupRenderState() const {
	if (!m_renderState) {
		m_renderState = m_state->clone();
	}
}

const globjects::ref_ptr<glannotations::AbstractRenderer>& glannotations::AbstractAnnotation::getRenderer() {
	return m_renderer;
}

bool glannotations::AbstractAnnotation::isOnNearplane() const {
	try {
		getRenderState()->asViewportState();
		return true;
	}
	catch (std::bad_cast&) {
		return false;
	}
}

glannotations::AbstractAnnotation::~AbstractAnnotation() {

}

void glannotations::AbstractAnnotation::prepareRenderer() {
	if (m_enabled == false) return;
	if (isDirty()) {
		m_renderer->prepare(this);
	}
}

void glannotations::AbstractAnnotation::prepareDraw() {
	if (m_enabled == false) return;
	getRenderState()->prepare();
}

globjects::ref_ptr<glannotations::SpaceObject> glannotations::AbstractAnnotation::getRespectiveSpaceObject() const {
	return m_respectiveSpaceObject;
}

void glannotations::AbstractAnnotation::setRespectiveSpaceObject(const globjects::ref_ptr<glannotations::SpaceObject>& spaceObject) {
	m_respectiveSpaceObject = spaceObject;
}

void glannotations::AbstractAnnotation::setCurrentFallback(unsigned val) {
	m_currentFallback = val;
}

unsigned glannotations::AbstractAnnotation::getCurrentFallback() const {
	return m_currentFallback;
}

void glannotations::AbstractAnnotation::enable() {
	m_enabled = true;
}

void glannotations::AbstractAnnotation::disable() {
	m_enabled = false;
}
