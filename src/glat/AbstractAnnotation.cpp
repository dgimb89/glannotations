#include <glat/AbstractAnnotation.h>

glat::AbstractAnnotation::AbstractAnnotation(glat::AbstractState* initialState, const glat::RendererFactory& factory) 
	: m_renderer(factory.createRenderer()) {
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
}

glat::AbstractState* glat::AbstractAnnotation::getState(unsigned statePosition /*= 0*/) {
	return m_states.at(statePosition);
}
