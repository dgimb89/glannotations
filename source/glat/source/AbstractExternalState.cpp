#include <glat/AbstractExternalState.h>
#include <glat/Styles/ExternalColor.h>

void glat::AbstractExternalState::setCamera(gloutils::Camera* camera) {
	m_camera = camera;
}

gloutils::Camera* glat::AbstractExternalState::getCamera() const {
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

	if (m_drawExternal) {
		m_externalPrimitive->setModelViewProjection(m_camProjection);
	}
	return m_camProjection;
}

glat::AbstractExternalState::AbstractExternalState(gloutils::Camera* camera) : AbstractState() {
	m_internalState = new glat::InternalState(glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f, 0.f, 0.f), camera);
	m_interpolation = 0.f;
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

glo::ref_ptr<glat::InternalState> glat::AbstractExternalState::getInternalAnnotation() {
	return m_internalState;
}

const glm::vec3& glat::AbstractExternalState::getLL() const {
	return m_internalState->getLL();
}

const glm::vec3& glat::AbstractExternalState::getUR() const {
	return m_internalState->getUR();
}

const glm::vec3& glat::AbstractExternalState::getLR() const {
	return m_internalState->getLR();
}
