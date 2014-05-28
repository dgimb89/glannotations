#include <glat/BoxReference.h>

void glat::BoxReference::updatePositioning(InternalState& state) {
	throw std::logic_error("The method or operation is not implemented.");
}

void glat::BoxReference::updatePositioning(InternalPathState& state) {
	throw std::logic_error("The method or operation is not implemented.");
}

glat::BoxReference::BoxReference(glm::vec3 widthSpan, glm::vec3 heightSpan, glm::vec3 depthSpan) {
	m_widthSpan = widthSpan;
	m_heightSpan = heightSpan;
	m_depthSpan = depthSpan;
}
