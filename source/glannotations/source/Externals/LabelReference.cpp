#include <glannotations/Externals/LabelReference.h>
#include <glannotations/States/QuadState.h>
#include <glannotations/States/SplineState.h>
#include <glannotations/Common/globals.h>

void glannotations::LabelReference::updatePositioning(QuadState& state) {
	glm::vec3 upVector = glm::normalize(glannotations::getUp(getBindingIndex())) * m_height;
	m_newLL = m_annotationCenter - (m_height + m_width) / 2.f;
	m_newLR = m_newLL + glm::normalize(glannotations::getRight(getBindingIndex())) * m_width;
	state.setExtends(m_newLL, m_newLR, m_newLR + upVector);
}

void glannotations::LabelReference::updatePositioning(SplineState& /*state*/) {
    throw std::logic_error("The method or operation is not implemented.");
	//todo:anne what do I have to do here?
}

void glannotations::LabelReference::setupExternalReference(const QuadState& state) {
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

void glannotations::LabelReference::draw() {
	updatePrismoid(m_newLL, m_newLR);
	AbstractExternalReference::draw();
}
