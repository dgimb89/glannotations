#include <glat/AbstractExternalState.h>
#include <glat/Styles/ExternalColor.h>

glow::ref_ptr<glat::InternalState> glat::AbstractExternalState::getInternalAnnotation() {
	return m_internalState;
}

void glat::AbstractExternalState::setCamera(glowutils::Camera* camera) {
	m_camera = camera;
}

glowutils::Camera* glat::AbstractExternalState::getCamera() const {
	return m_camera;
}

bool glat::AbstractExternalState::isDirty() const {
	if (m_camera->viewProjection() != m_camProjection) {
		return true;
	}
	return glat::AbstractState::isDirty();
}

const glm::mat4& glat::AbstractExternalState::getViewProjection() const {

	if (m_interpolation <= 0.75f) {
		m_camProjection = glm::mat4(m_camera->projection() * m_interpolation + m_camera->viewProjection() * (1.f - m_interpolation));
	}
	else {
		m_camProjection = glm::mat4() * ((m_interpolation - 0.75f) * 4.f) + (m_camera->projection() * 0.75f + m_camera->viewProjection() * 0.25f) * (1.f - ((m_interpolation - 0.75f) * 4.f));
	}

	m_externalPrimitive->setModelViewProjection(m_camProjection);
	return m_camProjection;
}

glat::AbstractExternalState::AbstractExternalState(glowutils::Camera* camera) : AbstractState() {
	setCamera(camera);
}

bool glat::AbstractExternalState::getDrawExternal() const {
	return m_drawExternal;
}

void glat::AbstractExternalState::setDrawExternal(bool drawExternal) {
	m_drawExternal = drawExternal;
}

void glat::AbstractExternalState::drawExternalPrimitives() const {
	if (getDrawExternal()) {
		m_externalPrimitive->draw();
	}
}

void glat::AbstractExternalState::setupExternalColor(const glat::Styling* externalColor) const {
	if (externalColor == nullptr) return;
	const glat::Styles::ExternalColor* externalColorStyle = reinterpret_cast<const glat::Styles::ExternalColor*>(externalColor);
	m_externalPrimitive->setColor(externalColorStyle->getColor());
}

void glat::AbstractExternalState::setupExternalPrimitives() const {
	setupExternalColor(getStyling("ExternalColor"));
}
