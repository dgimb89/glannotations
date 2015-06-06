#include <glannotations/LabelReference.h>
#include <glannotations/InternalState.h>
#include <glannotations/SplineState.h>
#include <glannotations/globals.h>

void glannotations::LabelReference::updatePositioning(InternalState& state) {
	glm::vec3 upVector = glm::normalize(glannotations::getUp(getBindingIndex())) * m_height;
	glm::vec3 newLL = m_annotationCenter - (m_height + m_width) / 2.f;
	glm::vec3 newLR = newLL + glm::normalize(glannotations::getRight(getBindingIndex())) * m_width;
	state.setExtends(newLL, newLR, newLR + upVector);
	updatePrismoid(newLL, newLR);
}

void glannotations::LabelReference::updatePositioning(SplineState& state) {
	//throw std::logic_error("The method or operation is not implemented.");
	//todo:anne what do I have to do here?
}

void glannotations::LabelReference::setupExternalReference(const InternalState& state) {
	AbstractExternalReference::setupExternalReference(state);
	m_annotationCenter = (state.getLL() + state.getLR()) / 2.f + (state.getUR() - state.getLR()) / 2.f;
	m_width = glm::distance(state.getLR(), state.getLL());
	m_height = glm::distance(state.getUR(), state.getLR());
}

void glannotations::LabelReference::setupExternalReference(const SplineState& state) {
	AbstractExternalReference::setupExternalReference(state);
	m_annotationCenter = state.getLL();
	m_width = 1.f;
	m_height = 1.f;
	//todo:anne put some meaningful values in here!
}

glannotations::LabelReference::LabelReference(glm::vec3 reference) : AbstractExternalReference(false) {
	auto prismoid = new glannotations::Prismoid;
	prismoid->setColor(glm::vec4(0.f, 0.f, 0.f, 1.f));
	prismoid->setReference(reference);
	m_externalPrimitive = prismoid;
}

void glannotations::LabelReference::updatePrismoid(const glm::vec3& a, const glm::vec3& b) {
	reinterpret_cast<glannotations::Prismoid*>(m_externalPrimitive.get())->setPosition(a, b);
}
