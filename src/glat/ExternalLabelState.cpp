#include <glat/ExternalLabelState.h>
#include <glat/AbstractRenderer.h>
#include <glat/Quad.h>

bool glat::ExternalLabelState::isValid() {
	return m_width > 0.f && m_height > 0.f;
}

void glat::ExternalLabelState::interpolate(const AbstractAnnotation& annotation, AbstractState* secondState, float interpolate) const {
	throw std::logic_error("The method or operation is not implemented.");
}

void glat::ExternalLabelState::interpolate(const AbstractAnnotation& annotation, const ViewportState& viewState, float interpolate) const {
	throw std::logic_error("The method or operation is not implemented.");
}

void glat::ExternalLabelState::interpolate(const AbstractAnnotation& annotation, const InternalState& internalState, float interpolate) const {
	throw std::logic_error("The method or operation is not implemented.");
}

void glat::ExternalLabelState::interpolate(const AbstractAnnotation& annotation, const ExternalBoxState& externalState, float interpolate) const {
	throw std::logic_error("The method or operation is not implemented.");
}

void glat::ExternalLabelState::draw(const AbstractRenderer& renderer) const {
	renderer.drawSetupState(*this);
}

void glat::ExternalLabelState::updateInternalPosition() const {
	glm::vec3 heightSpan = glm::normalize(m_camera->up()) * m_height;
	glm::vec3 widthSpan = glm::cross(heightSpan, glm::normalize(m_camera->eye() - m_camera->center())) * 0.5f * m_width;
	m_internalState->setExtends(m_center - widthSpan, m_center + widthSpan, m_center + widthSpan + heightSpan);
	reinterpret_cast<glat::Quad*>(m_externalPrimitive.get())->setPosition(m_reference, getLL(), getLL() + 0.05f*glm::cross(glm::normalize(m_camera->eye() - m_camera->center()),widthSpan), getViewProjection());
}

glat::ExternalLabelState::ExternalLabelState(glm::vec3 reference, glm::vec3 center, float width, float height, glowutils::Camera* camera, bool drawBox /*= true*/) : glat::AbstractExternalState(camera) {
	m_externalPrimitive = new glat::Quad();
	setDrawExternal(drawBox);
	setReference(reference);
	setExtends(center, width, height);
}

void glat::ExternalLabelState::setReference(glm::vec3 reference) {
	m_reference = reference;
}

void glat::ExternalLabelState::setExtends(glm::vec3 center, float width, float height) {
	m_center = center;
	m_width = width;
	m_height = height;
	updateInternalPosition();
}

