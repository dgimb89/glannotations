#include <glat/AbstractAnnotation.h>
#include <glat/ViewportState.h>
#include <glat/InternalState.h>
#include <glat/ExternalBoxState.h>
#include <glat/Styles/ExternalColor.h>

glat::AbstractAnnotation::AbstractAnnotation(const glow::ref_ptr<glat::AbstractState>& state) {
	setState(state);
}

void glat::AbstractAnnotation::draw() {
	m_renderer->draw(this);
	setDirty(false);
}

glow::ref_ptr<glat::AbstractState> glat::AbstractAnnotation::getState() const {
	return m_state;
}

void glat::AbstractAnnotation::setState(const glow::ref_ptr<glat::AbstractState>& state) {
	setDirty(true);
	m_state = state;
}

glow::ref_ptr<glat::AbstractState> glat::AbstractAnnotation::getRenderState() const {
	if (m_interpolatedState) {
		return m_interpolatedState;
	}
	return m_state;
}

void glat::AbstractAnnotation::resetInterpolation() {
	setDirty(true);
	m_interpolatedState = m_state;
}

void glat::AbstractAnnotation::interpolateState(const InternalState& mixState, float mix) {
	setDirty(true);
	m_interpolatedState = m_state->interpolateWith(mixState, mix);
}

void glat::AbstractAnnotation::interpolateState(const InternalPathState& mixState, float mix) {
	setDirty(true);
	m_interpolatedState = m_state->interpolateWith(mixState, mix);
}

void glat::AbstractAnnotation::interpolateState(const ViewportState& mixState, float mix) {
	setDirty(true);
	m_interpolatedState = m_state->interpolateWith(mixState, mix);
}
