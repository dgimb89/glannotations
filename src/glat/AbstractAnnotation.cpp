#include <glat/AbstractAnnotation.h>

glat::AbstractAnnotation::AbstractAnnotation(glat::RendererFactory* factory) :
	m_renderer(factory->createRenderer()),
	m_dirty(true) {
}

bool glat::AbstractAnnotation::isDirty() {
	return m_dirty;
}

void glat::AbstractAnnotation::setDirty(bool dirty) {
	m_dirty = dirty;
}

void glat::AbstractAnnotation::setState(float statePhase) {
	m_statePhase = statePhase;
	setDirty(true);
}

void glat::AbstractAnnotation::addState(glat::AbstractState* state) {
	m_states.push_back(std::shared_ptr<AbstractState>(state));
	setDirty(true);
}

void glat::AbstractAnnotation::draw() {
	m_renderer->draw(this);
}