#include <glat/ExternalBoxState.h>
#include <glat/AbstractRenderer.h>
#include <glat/Box.h>

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
	float scalarFrontBack = std::fabsf(glm::dot(glm::normalize(glm::cross(m_heightSpan, m_widthSpan)), lookAt));
	// left & right faces
	float scalarLeftRight = std::fabsf(glm::dot(glm::normalize(glm::cross(m_heightSpan, m_depthSpan)), lookAt));
	// bottom & up faces
	float scalarBottomUp = std::fabsf(glm::dot(glm::normalize(glm::cross(m_depthSpan, m_widthSpan)), lookAt));

	if (scalarFrontBack > scalarBottomUp && scalarFrontBack > scalarLeftRight) {
		// Front-Back-Faces
		updatePositions(m_llf, m_widthSpan, m_heightSpan, m_depthSpan, m_camera->eye());
		return;
	}

	if (scalarLeftRight > scalarBottomUp) {
		// Left-Right-Faces
		updatePositions(m_llf + m_widthSpan, m_depthSpan, m_heightSpan, -m_widthSpan, m_camera->eye());
		return;
	}

	// Bottom-Up-Faces
	updatePositions(m_llf + m_heightSpan, m_widthSpan, m_depthSpan, -m_heightSpan, m_camera->eye());
	return;
}

inline void glat::ExternalBoxState::updatePositions(const glm::vec3 ll, const glm::vec3 widthSpan, const glm::vec3 heightSpan, const glm::vec3 depthSpan, const glm::vec3 eye) const {
	// this is not the actual distance but as we know the current face is our best fit (approx. orthogonal to lookAt vector) and other face is parallel, we can get away with just checking 1 point
	if (glm::length(ll - eye) < glm::length((ll + depthSpan) - eye)) {
		m_internalLL = ll - (depthSpan * 0.01f);
		m_internalLR = m_internalLL + widthSpan;
		m_internalUR = m_internalLR + heightSpan;
		return;
	} 

	// back face
	m_internalLL = ll + widthSpan + (depthSpan*1.01f);
	m_internalLR = m_internalLL - widthSpan;
	m_internalUR = m_internalLR + heightSpan;
}
