#include <glat/BoxReference.h>
#include <glat/InternalPathState.h>
#include <glat/InternalState.h>

void glat::BoxReference::updatePositioning(InternalState& state) {
	if (isDirty() && !isPositioningOnly()) {
		// Todo: consider anchors
		glm::vec3 centerToCorner = m_depthSpan / 2.f + 
			(m_widthSpan - glm::distance(state.getLR(), state.getLL())) / 2.f + 
			(m_heightSpan - glm::distance(state.getUR(), state.getLR())) / 2.f;
		m_box->setPosition(state.getLL() - centerToCorner, state.getUR() + centerToCorner);
	}
	setDirty(false);
}

void glat::BoxReference::updatePositioning(InternalPathState& state) {
	throw std::logic_error("The method or operation is not implemented.");
}

glat::BoxReference::BoxReference(glm::vec3 widthSpan, glm::vec3 heightSpan, glm::vec3 depthSpan, glowutils::Camera* camera, bool onlyPositioning)
	: glat::AbstractExternalReference(camera, onlyPositioning){
	setWidth(widthSpan);
	setHeight(heightSpan);
	setDepth(depthSpan);
	setDirty(true);
}

void glat::BoxReference::draw() {
	m_box->setModelViewProjection(m_camera->viewProjection());
	m_box->draw();
}

void glat::BoxReference::setWidth(glm::vec3 widthSpan) {
	setDirty(true);
	m_widthSpan = widthSpan;
}

void glat::BoxReference::setHeight(glm::vec3 heightSpan) {
	setDirty(true);
	m_heightSpan = heightSpan;
}

void glat::BoxReference::setDepth(glm::vec3 depthSpan) {
	setDirty(true);
	m_depthSpan = depthSpan;
}
