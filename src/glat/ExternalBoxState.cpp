#include <glat/ExternalBoxState.h>
#include <glat/AbstractRenderer.h>
#include <glat/Box.h>

const float NearPlacementOffset = 0.01f;

bool glat::ExternalBoxState::isValid() {
	// all spanning vectors have to be linear independent
	return	glm::cross(m_widthSpan, m_heightSpan).length() > 0.0
			&& glm::cross(m_widthSpan, m_depthSpan).length() > 0.0
			&& glm::cross(m_heightSpan, m_depthSpan).length() > 0.0;
}

void glat::ExternalBoxState::interpolate(const AbstractAnnotation& annotation, AbstractState* secondState, float interpolate) const {
	// TODO
	throw std::logic_error("The method or operation is not implemented.");
}

void glat::ExternalBoxState::interpolate(const AbstractAnnotation& annotation, const ViewportState& viewState, float interpolate) const {
	// TODO
	throw std::logic_error("The method or operation is not implemented.");
}

void glat::ExternalBoxState::interpolate(const AbstractAnnotation& annotation, const InternalState& internalState, float interpolate) const {
	// TODO
	throw std::logic_error("The method or operation is not implemented.");
}

void glat::ExternalBoxState::draw(const AbstractRenderer& renderer) const {
	renderer.drawSetupState(*this);
}

void glat::ExternalBoxState::setExtends(glm::vec3 llf, glm::vec3 widthSpan, glm::vec3 heightSpan, glm::vec3 depthSpan) {
	m_llf = llf;
	m_widthSpan = widthSpan;
	m_heightSpan = heightSpan;
	m_depthSpan = depthSpan;
	reinterpret_cast<glat::Box*>(m_externalPrimitive.get())->setPosition(llf, llf + widthSpan + heightSpan + depthSpan);
}

glat::ExternalBoxState::ExternalBoxState(glm::vec3 llf, glm::vec3 widthSpan, glm::vec3 heightSpan, glm::vec3 depthSpan, glowutils::Camera* camera, bool drawBox /*= true*/)
: AbstractExternalState(camera) {
	m_externalPrimitive = new glat::Box();
	setExtends(llf, widthSpan, heightSpan, depthSpan);
	setDrawExternal(drawBox);
}

const glm::vec3& glat::ExternalBoxState::getLL() const {
	return m_internalLL;
}

const glm::vec3& glat::ExternalBoxState::getUR() const {
	return m_internalUR;
}

const glm::vec3& glat::ExternalBoxState::getLR() const {
	return m_internalLR;
}

void glat::ExternalBoxState::updateInternalPosition() const {
	glm::vec3 lookAt = glm::normalize(m_camera->center() - m_camera->eye());
	// first choose which side the annotation should be rendered on by evaluate lowest deviation face-normal to camera-lookAt 
	// front & back faces
	float scalarFrontSide = glm::dot(glm::normalize(glm::cross(m_heightSpan, m_widthSpan)), lookAt);
	// left & right faces
	float scalarLeftSide = glm::dot(glm::normalize(glm::cross(m_heightSpan, m_depthSpan)), lookAt);
	// bottom & up faces
	float scalarBottomSide = glm::dot(glm::normalize(glm::cross(m_depthSpan, m_widthSpan)), lookAt);

	// first look which angle is the smallest angle
	if (std::fabs(scalarFrontSide) > std::fabs(scalarBottomSide) && std::fabs(scalarFrontSide) > std::fabs(scalarLeftSide)) {
		// Front-Back-Faces
		updatePositions(m_llf, m_widthSpan, m_heightSpan, m_depthSpan, scalarFrontSide < 0.f);
		return;
	}

	if (std::fabs(scalarLeftSide) > std::fabs(scalarBottomSide)) {
		// Left-Right-Faces
		updatePositions(m_llf + m_widthSpan, m_depthSpan, m_heightSpan, -m_widthSpan, scalarLeftSide < 0.f);
		return;
	}

	// Bottom-Up-Faces
	updatePositions(m_llf + m_heightSpan, m_widthSpan, m_depthSpan, -m_heightSpan, scalarBottomSide < 0.f);
	return;
}

inline void glat::ExternalBoxState::updatePositions(glm::vec3 ll, glm::vec3 widthSpan, glm::vec3 heightSpan, glm::vec3 depthSpan, bool backside) const {
	if (backside) {
		ll = ll + depthSpan + widthSpan;
		widthSpan = -widthSpan;
		depthSpan = -depthSpan;
	}

	m_internalLL = ll - (depthSpan * NearPlacementOffset);
	m_internalLR = m_internalLL + widthSpan;
	m_internalUR = m_internalLR + heightSpan;
	return;
}
