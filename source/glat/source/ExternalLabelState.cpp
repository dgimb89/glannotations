#include <glat/ExternalLabelState.h>
#include <glat/AbstractRenderer.h>
#include <glat/Prismoid.h>
#include <glat/Utility.h>

bool glat::ExternalLabelState::isValid() {
	return m_width > 0.f && m_height > 0.f;
}

void glat::ExternalLabelState::draw(const AbstractRenderer& renderer) {
	renderer.drawSetupState(*this);
}

void glat::ExternalLabelState::updateInternalPosition() const {
	glm::vec3 heightSpan = glm::normalize(m_camera->up()) * m_height;
	glm::vec3 widthSpan = glm::cross(heightSpan, glm::normalize(m_camera->eye() - m_camera->center())) * 0.5f * m_width;
	m_internalState->setExtends(m_center - widthSpan, m_center + widthSpan, m_center + widthSpan + heightSpan);

	std::vector<glm::vec3> linePoints({ m_reference });
	glat::Utility::computeNextNearestPoint(2, m_reference, std::vector<glm::vec3>({ m_internalState->getLL(), m_internalState->getLR() }), linePoints);
	reinterpret_cast<glat::Prismoid*>(m_externalPrimitive.get())->setPosition(linePoints, getViewProjection());
}

glat::ExternalLabelState::ExternalLabelState(glm::vec3 reference, glm::vec3 center, float width, float height, glowutils::Camera* camera, bool drawBox /*= true*/) : glat::AbstractExternalState(camera) {
	m_externalPrimitive = new glat::Prismoid;
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

