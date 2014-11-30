#include <glannotations/AbstractExternalState.h>
#include <glannotations/Styles/ExternalColor.h>

void glannotations::AbstractExternalState::setCamera(gloutils::Camera* camera) {
	m_camera = camera;
}

gloutils::Camera* glannotations::AbstractExternalState::getCamera() const {
	return m_camera;
}

bool glannotations::AbstractExternalState::isDirty() const {
	if (m_camera->viewProjection() != m_camProjection) {
		return true;
	}
	return glannotations::AbstractState::isDirty();
}

const glm::mat4& glannotations::AbstractExternalState::getViewProjection() const {

	if (m_interpolation <= 0.75f) {
		m_camProjection = glm::mat4(m_camera->projection() * m_interpolation + m_camera->viewProjection() * (1.f - m_interpolation));
	}
	else {
		m_camProjection = glm::mat4() * ((m_interpolation - 0.75f) * 4.f) + (m_camera->projection() * 0.75f + m_camera->viewProjection() * 0.25f) * (1.f - ((m_interpolation - 0.75f) * 4.f));
	}

	if (m_drawExternal) {
		m_externalPrimitive->setModelViewProjection(m_camProjection);
	}
	return m_camProjection;
}

glannotations::AbstractExternalState::AbstractExternalState(gloutils::Camera* camera) : AbstractState() {
	m_internalState = new glannotations::InternalState(glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f, 0.f, 0.f), camera);
	m_interpolation = 0.f;
	setCamera(camera);
}

bool glannotations::AbstractExternalState::getDrawExternal() const {
	return m_drawExternal;
}

void glannotations::AbstractExternalState::setDrawExternal(bool drawExternal) {
	m_drawExternal = drawExternal;
}

void glannotations::AbstractExternalState::drawExternalPrimitives() const {
	if (getDrawExternal()) {
		m_externalPrimitive->draw();
	}
}

void glannotations::AbstractExternalState::setupExternalColor(const glannotations::Styling* externalColor) const {
	if (externalColor == nullptr) return;
	const glannotations::Styles::ExternalColor* externalColorStyle = reinterpret_cast<const glannotations::Styles::ExternalColor*>(externalColor);
	m_externalPrimitive->setColor(externalColorStyle->getColor());
}

void glannotations::AbstractExternalState::setupExternalPrimitives() const {
	setupExternalColor(getStyling("ExternalColor"));
}

globjects::ref_ptr<glannotations::InternalState> glannotations::AbstractExternalState::getInternalAnnotation() {
	return m_internalState;
}

const glm::vec3& glannotations::AbstractExternalState::getLL() const {
	return m_internalState->getLL();
}

const glm::vec3& glannotations::AbstractExternalState::getUR() const {
	return m_internalState->getUR();
}

const glm::vec3& glannotations::AbstractExternalState::getLR() const {
	return m_internalState->getLR();
}
