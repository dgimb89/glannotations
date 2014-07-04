#include <glat/LabelReference.h>
#include <glat/InternalState.h>
#include <glat/PathState.h>
#include <glat/globals.h>

void glat::LabelReference::updatePositioning(InternalState& state) {
	glm::vec3 upVector = glm::normalize(glat::getUp(getBindingIndex())) * m_height;
	glm::vec3 newLL = m_annotationCenter - (m_height + m_width) / 2.f;
	glm::vec3 newLR = newLL + glm::normalize(glat::getRight(getBindingIndex())) * m_width;
	state.setExtends(newLL, newLR, newLR + upVector);
	updatePrismoid(newLL, newLR);
}

void glat::LabelReference::updatePositioning(PathState& state) {
	throw std::logic_error("The method or operation is not implemented.");
}

void glat::LabelReference::setupExternalReference(const InternalState& state) {
	AbstractExternalReference::setupExternalReference(state);
	m_annotationCenter = (state.getLL() + state.getLR()) / 2.f + (state.getUR() - state.getLR()) / 2.f;
	m_width = glm::distance(state.getLR(), state.getLL());
	m_height = glm::distance(state.getUR(), state.getLR());
	reinterpret_cast<glat::Prismoid*>(m_externalPrimitive.get())->setReference(m_annotationCenter);
}

void glat::LabelReference::setupExternalReference(const PathState& state) {
	throw std::logic_error("The method or operation is not implemented.");
}

glat::LabelReference::LabelReference(glm::vec3 reference) : AbstractExternalReference(false) {
	m_externalPrimitive = new glat::Prismoid;
	m_externalPrimitive->setColor(glm::vec4(0.f, 0.f, 0.f, 1.f));
}

inline void glat::LabelReference::updatePrismoid(const glm::vec3& a, const glm::vec3& b) {
	reinterpret_cast<glat::Prismoid*>(m_externalPrimitive.get())->setPosition(a, b);
}
