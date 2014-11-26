#include <glannotations/LabelReference.h>
#include <glannotations/InternalState.h>
#include <glannotations/PathState.h>
#include <glannotations/globals.h>

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
}

void glat::LabelReference::setupExternalReference(const PathState& state) {
	throw std::logic_error("The method or operation is not implemented.");
}

glat::LabelReference::LabelReference(glm::vec3 reference) : AbstractExternalReference(false) {
	auto prismoid = new glat::Prismoid;
	prismoid->setColor(glm::vec4(0.f, 0.f, 0.f, 1.f));
	prismoid->setReference(reference);
	m_externalPrimitive = prismoid;
}

inline void glat::LabelReference::updatePrismoid(const glm::vec3& a, const glm::vec3& b) {
	reinterpret_cast<glat::Prismoid*>(m_externalPrimitive.get())->setPosition(a, b);
}
