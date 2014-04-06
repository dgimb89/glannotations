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
	m_camProjection = m_camera->viewProjection();
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
