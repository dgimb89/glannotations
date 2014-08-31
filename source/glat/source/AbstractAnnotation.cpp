#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <glat/AbstractAnnotation.h>
#include <glat/ViewportState.h>
#include <glat/InternalState.h>
#include <glat/PathState.h>
#include <glat/Styles/ExternalColor.h>

glat::AbstractAnnotation::AbstractAnnotation(const glo::ref_ptr<glat::AbstractState>& state) {
	setState(state);
}

void glat::AbstractAnnotation::draw() {
	m_renderer->draw(this);
}

glo::ref_ptr<glat::AbstractState> glat::AbstractAnnotation::getState() const {
	// todo: implement observer pattern in case of state changes
	m_renderState = nullptr;
	return m_state;
}

void glat::AbstractAnnotation::setState(const glo::ref_ptr<glat::AbstractState>& state) {
	m_state = state;
	m_renderState = nullptr;
}

glo::ref_ptr<glat::AbstractState> glat::AbstractAnnotation::getRenderState() const {
	setupRenderState();
	return m_renderState;
}

void glat::AbstractAnnotation::resetInterpolation() {
	m_renderState = nullptr;
}

void glat::AbstractAnnotation::interpolateState(const InternalState& mixState, float mix) {
	m_renderState = nullptr;
	setupRenderState();
	m_renderState = m_state->interpolateWith(mixState, mix);
}

void glat::AbstractAnnotation::interpolateState(const PathState& mixState, float mix) {
	m_renderState = nullptr;
	setupRenderState();
	m_renderState = m_state->interpolateWith(mixState, mix);
}

void glat::AbstractAnnotation::interpolateState(const ViewportState& mixState, float mix) {
	m_renderState = nullptr;
	setupRenderState();
	m_renderState = m_state->interpolateWith(mixState, mix);
}

void glat::AbstractAnnotation::setupRenderState() const {
	if (!m_renderState) {
		m_renderState = m_state->clone();
	}
}

const glo::ref_ptr<glat::AbstractRenderer>& glat::AbstractAnnotation::getRenderer() {
	return m_renderer;
}

bool glat::AbstractAnnotation::isOnNearplane() const {
	try {
		getRenderState()->asViewportState();
		return true;
	}
	catch (std::bad_cast& e) {
		return false;
	}
}
