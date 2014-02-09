#include <glat/AbstractAnnotation.h>

glat::AbstractAnnotation::AbstractAnnotation(glat::AbstractState* initialState) {
	addState(initialState);
}

void glat::AbstractAnnotation::setState(float statePhase) {
	setDirty(true);
	m_statePhase = statePhase;
}

void glat::AbstractAnnotation::addState(glat::AbstractState* state) {
	setDirty(true);
	m_states.push_back(state);
}

void glat::AbstractAnnotation::draw() {
	m_renderer->draw(this);
	setDirty(false);
}

glow::ref_ptr<glat::AbstractState> glat::AbstractAnnotation::getState(unsigned statePosition /*= 0*/) {
	return m_states.at(statePosition);
}