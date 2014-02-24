#include <glat/AbstractAnnotation.h>
#include <glat/ViewportState.h>
#include <glat/InternalState.h>

glat::AbstractAnnotation::AbstractAnnotation(glat::AbstractState* initialState) {
	addState(initialState);
}

void glat::AbstractAnnotation::setState(unsigned statePosition) {
	setDirty(true);
	if (statePosition < m_states.size())
		m_activeState = m_states.at(statePosition);
	m_activeState->setDirty(true);
}

void glat::AbstractAnnotation::setInterpolatedState(unsigned firstStatePos, unsigned secondStatePos, float interpolate) {
	setDirty(true);
	glow::ref_ptr<glat::AbstractState> firstState = m_states.at(firstStatePos);
	glow::ref_ptr<glat::AbstractState> secondState = m_states.at(secondStatePos);
	firstState->interpolate(*this, secondState, interpolate);
}

unsigned glat::AbstractAnnotation::addState(glat::AbstractState* state) {
	setDirty(true);
	if (m_activeState.get() == NULL) m_activeState = state;
	m_states.push_back(state);
	return m_states.size();
}

void glat::AbstractAnnotation::draw() {
	m_renderer->draw(this);
	setDirty(false);
}

glow::ref_ptr<glat::AbstractState> glat::AbstractAnnotation::getState() {
	return m_activeState;
}

void glat::AbstractAnnotation::interpolate(const ViewportState& firstState, const ViewportState& secondState, float interpolate) const {
	glm::vec2 newLL = firstState.getLL() * (1.f - interpolate) + secondState.getLL() * interpolate;
	glm::vec2 newUR = firstState.getUR() * (1.f - interpolate) + secondState.getUR() * interpolate;
	m_activeState = new ViewportState(newLL, newUR);
}

void glat::AbstractAnnotation::interpolate(const ViewportState& firstState, const InternalState& secondState, float interpolate) const {
	glm::vec3 newLL = glm::vec3(firstState.getLL(), 0.f) * (1.f - interpolate) + secondState.getLL() * interpolate;
	glm::vec3 newLR = glm::vec3(firstState.getLR(), 0.f) * (1.f - interpolate) + secondState.getLR() * interpolate;
	glm::vec3 newUR = glm::vec3(firstState.getUR(), 0.f) * (1.f - interpolate) + secondState.getUR() * interpolate;
	m_activeState = new InternalState(newLL, newLR, newUR, secondState.getCamera(), 1.f-interpolate);
}

void glat::AbstractAnnotation::interpolate(const InternalState& firstState, const InternalState& secondState, float interpolate) const {
	glm::vec3 newLL = firstState.getLL() * interpolate + secondState.getLL() * (1.f - interpolate);
	glm::vec3 newLR = firstState.getLR() * interpolate + secondState.getLR() * (1.f - interpolate);
	glm::vec3 newUR = firstState.getUR() * interpolate + secondState.getUR() * (1.f - interpolate);
	m_activeState = new InternalState(newLL, newLR, newUR, firstState.getCamera());
}