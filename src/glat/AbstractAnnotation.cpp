#include <glat/AbstractAnnotation.h>
#include <glat/ViewportState.h>
#include <glat/InternalState.h>
#include <glat/ExternalBoxState.h>
#include <glat/Styles/ExternalColor.h>

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
	m_activeState = new InternalState(newLL, newLR, newUR, secondState.getCamera(), 1.f - interpolate);
}

void glat::AbstractAnnotation::interpolate(const InternalState& firstState, const InternalState& secondState, float interpolate) const {
	glm::vec3 newLL = firstState.getLL() * (1.f - interpolate) + secondState.getLL() * interpolate;
	glm::vec3 newLR = firstState.getLR() * (1.f - interpolate) + secondState.getLR() * interpolate;
	glm::vec3 newUR = firstState.getUR() * (1.f - interpolate) + secondState.getUR() * interpolate;
	m_activeState = new InternalState(newLL, newLR, newUR, firstState.getCamera());
}

void glat::AbstractAnnotation::interpolate(const ViewportState& firstState, const ExternalBoxState& secondState, float interpolate) const {

}

void glat::AbstractAnnotation::interpolate(const InternalState& firstState, const ExternalBoxState& secondState, float interpolate) const {

	if (interpolate <= 0.5f) {
		glow::ref_ptr<glat::ExternalBoxState> state = new ExternalBoxState(firstState.getLL() * 0.5f + secondState.getLLF() * 0.5f, secondState.getWidth(), secondState.getHeight(), secondState.getDepth(), firstState.getCamera(), true);
		state->updateInternalPosition();

		glm::vec3 newLL = firstState.getLL() * (1.f - interpolate * 2.f) + state->getLL() * interpolate * 2.f;
		glm::vec3 newLR = firstState.getLR() * (1.f - interpolate * 2.f) + state->getLR() * interpolate * 2.f;
		glm::vec3 newUR = firstState.getUR() * (1.f - interpolate * 2.f) + state->getUR() * interpolate * 2.f;
		m_activeState = new InternalState(newLL, newLR, newUR, secondState.getCamera());
	}
	else {
		glm::vec3 newLLF = firstState.getLL() * (1.f - interpolate) + secondState.getLLF() * interpolate;
		glm::vec3 newWidth = secondState.getWidth();
		glm::vec3 newHeight = secondState.getHeight();
		glm::vec3 newDepth = secondState.getDepth();

		const glat::Styling* secondStyling = secondState.getStyling("ExternalColor");
		glm::vec4 firstColor, secondColor;
		firstColor = glm::vec4(0.f);

		if (secondStyling != nullptr)
			secondColor = dynamic_cast<const glat::Styles::ExternalColor*>(secondStyling)->getColor();
		else
			secondColor = glm::vec4(0.f);

		glm::vec4 newColor = firstColor * (2.f - interpolate*2.f) + secondColor * (interpolate * 2.f - 1.f);

		m_activeState = new ExternalBoxState(newLLF, newWidth, newHeight, newDepth, firstState.getCamera(), true);
		m_activeState->setStyling(new glat::Styles::ExternalColor(newColor));
	}
}

void glat::AbstractAnnotation::interpolate(const ExternalBoxState& firstState, const ExternalBoxState& secondState, float interpolate) const {
	glm::vec3 newLLF = firstState.getLLF() * (1.f - interpolate) + secondState.getLLF() * interpolate;
	glm::vec3 newHeight = firstState.getHeight() * (1.f - interpolate) + secondState.getHeight() * interpolate;
	glm::vec3 newWidth = firstState.getWidth() * (1.f - interpolate) + secondState.getWidth() * interpolate;
	glm::vec3 newDepth = firstState.getDepth() * (1.f - interpolate) + secondState.getDepth() * interpolate;

	const glat::Styling* firstStyling = firstState.getStyling("ExternalColor");
	const glat::Styling* secondStyling = secondState.getStyling("ExternalColor");
	glm::vec4 firstColor, secondColor;

	if (firstStyling != nullptr)
		firstColor = dynamic_cast<const glat::Styles::ExternalColor*>(firstStyling)->getColor();
	else
		firstColor = glm::vec4(0.f);

	if (secondStyling != nullptr)
		secondColor = dynamic_cast<const glat::Styles::ExternalColor*>(secondStyling)->getColor();
	else
		secondColor = glm::vec4(0.f);

	glm::vec4 newColor = firstColor * (1.f - interpolate) + secondColor * interpolate;

	m_activeState = new ExternalBoxState(newLLF, newWidth, newHeight, newDepth, firstState.getCamera(), true);
	m_activeState->setStyling(new glat::Styles::ExternalColor(newColor));
}