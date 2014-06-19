#include <iostream>

#include <glat/BoxReference.h>
#include <glat/PathState.h>
#include <glat/InternalState.h>
#include <glat/Segment.h>
#include <glat/Triangle.h>

void glat::BoxReference::updatePositioning(InternalState& state) {
	Segment camToBoxCenter(m_camera->eye(), m_frontLLF + (m_widthSpan + m_heightSpan + m_depthSpan) / 2.f);
	glm::vec3 intersection;
	glm::vec3 widthSpan;
	glm::vec3 heightSpan;
	//front
	if (intersectionSegmentQuad(camToBoxCenter, m_frontLLF, m_frontLLF + m_heightSpan, m_frontLLF + m_widthSpan, m_frontLLF + m_widthSpan + m_heightSpan, intersection)){
		widthSpan = glm::vec3(0.f, 0.f, 1.f);
		heightSpan = glm::vec3(0.f, 0.5f, 0.f);
	}
	// back
	else if (intersectionSegmentQuad(camToBoxCenter, m_frontLLF + m_depthSpan + m_widthSpan, m_frontLLF + m_depthSpan + m_widthSpan + m_heightSpan, m_frontLLF + m_depthSpan, m_frontLLF + m_depthSpan + m_heightSpan, intersection)) {
		widthSpan = glm::vec3(0.f, 0.f, -1.f);
		heightSpan = glm::vec3(0.f, 0.5f, 0.f);
	}
	// right
	else if (intersectionSegmentQuad(camToBoxCenter, m_frontLLF + m_widthSpan, m_frontLLF + m_widthSpan + m_heightSpan, m_frontLLF + m_widthSpan + m_depthSpan, m_frontLLF + m_widthSpan + m_depthSpan + m_heightSpan, intersection)) {
		widthSpan = glm::vec3(1.f, 0.f, 0.f);
		heightSpan = glm::vec3(0.f, 0.5f, 0.f);
	}
	// left
	else if (intersectionSegmentQuad(camToBoxCenter, m_frontLLF + m_depthSpan, m_frontLLF + m_depthSpan + m_heightSpan, m_frontLLF, m_frontLLF + m_heightSpan, intersection)) {
		widthSpan = glm::vec3(-1.f, 0.f, 0.f);
		heightSpan = glm::vec3(0.f, 0.5f, 0.f);
	}
	// top
	else if (intersectionSegmentQuad(camToBoxCenter, m_frontLLF + m_heightSpan, m_frontLLF + m_heightSpan + m_depthSpan, m_frontLLF + m_heightSpan + m_widthSpan, m_frontLLF + m_heightSpan + m_widthSpan + m_depthSpan, intersection)) {
		widthSpan = glm::vec3(0.f, 0.f, 1.f);
		heightSpan = glm::vec3(0.5f, 0.f, 0.f);
	}
	// bottom
	else if (intersectionSegmentQuad(camToBoxCenter, m_frontLLF + m_depthSpan, m_frontLLF, m_frontLLF + m_widthSpan + m_depthSpan, m_frontLLF + m_widthSpan, intersection)) {
		widthSpan = glm::vec3(0.f, 0.f, 1.f);
		heightSpan = glm::vec3(-0.5f, 0.f, 0.f);
	}
	state.setExtends(intersection - widthSpan - heightSpan, intersection + widthSpan - heightSpan, intersection + widthSpan + heightSpan);
}

void glat::BoxReference::updatePositioning(PathState& state) {
	throw std::logic_error("The method or operation is not implemented.");
}

glat::BoxReference::BoxReference(glm::vec3 widthSpan, glm::vec3 heightSpan, glm::vec3 depthSpan, glowutils::Camera* camera, bool onlyPositioning /* = true */)
	: glat::AbstractExternalReference(camera, onlyPositioning) {
	setWidth(widthSpan);
	setHeight(heightSpan);
	setDepth(depthSpan);
	setDirty(true);
}

void glat::BoxReference::draw() {
	if (!isPositioningOnly()) {
		m_box->setModelViewProjection(m_camera->viewProjection());
		gl::glDepthMask(gl::GL_FALSE);
		m_box->draw();
		gl::glDepthMask(gl::GL_TRUE);
	}
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

void glat::BoxReference::setupExternalReference(const InternalState& state) {
	if (isDirty() && !isPositioningOnly()) {
		m_box = new glat::Box;
		m_box->setColor(glm::vec4(1.f, 0.f, 0.f, .5f));
		glm::vec3 centerToCorner = (m_widthSpan - (state.getLR() - state.getLL())) / 2.f +
			(m_heightSpan - (state.getUR() - state.getLR())) / 2.f;
		m_box->setPosition(state.getLL() - centerToCorner, state.getUR() + centerToCorner + m_depthSpan);
	}
	m_frontLLF = state.getLL() - (m_widthSpan - (state.getLR() - state.getLL())) / 2.f - (m_heightSpan - (state.getUR() - state.getLR())) / 2.f;
	setDirty(false);
}

void glat::BoxReference::setupExternalReference(const PathState& state) {
	throw std::logic_error("The method or operation is not implemented.");
}

inline bool glat::BoxReference::intersectionSegmentQuad(const glat::Segment& ray, const glm::vec3& a, const glm::vec3& b, const glm::vec3& c, const glm::vec3& d, glm::vec3& point) {
	return ray.intersect(Triangle(a, b, c), point) || ray.intersect(Triangle(c,b,d), point);
}
