#define _USE_MATH_DEFINES
#include <cmath>
#include <glbinding/gl/boolean.h>
#include <glbinding/gl/functions.h>
#include <algorithm>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

#include <glannotations/FlagReference.h>
#include <glannotations/Rect.h>
#include <glannotations/AbstractAnnotation.h>
#include <glannotations/PathState.h>
#include <glannotations/InternalState.h>
#include <glannotations/Utility/Segment.h>
#include <glannotations/Utility/Triangle.h>

const float MAX_ROTATION_ANGLE = M_PI * 45.f / 180.f;
const float ANTI_FLICKERING = 0.01f;

void glannotations::FlagReference::updatePositioning(InternalState& state) {
	glm::vec3 center = m_frontLLF + (m_widthSpan + m_heightSpan + m_depthSpan) / 2.f;
	Utility::Segment camToBoxCenter(glannotations::getEye(getBindingIndex()), center);
	glm::vec3 intersection, widthSpan, heightSpan;
	glm::vec3 start;
	float hOverflow;

	//front
	if (intersectionSegmentQuad(camToBoxCenter, m_frontLLF, m_frontLLF + m_heightSpan, m_frontLLF + m_widthSpan, m_frontLLF + m_widthSpan + m_heightSpan, hOverflow)) {
		widthSpan = m_widthSpan;
		heightSpan = m_heightSpan;
		start = m_frontLLF + m_widthSpan;
	}
	// back
	else if (intersectionSegmentQuad(camToBoxCenter, m_frontLLF + m_depthSpan + m_widthSpan, m_frontLLF + m_depthSpan + m_widthSpan + m_heightSpan, m_frontLLF + m_depthSpan, m_frontLLF + m_depthSpan + m_heightSpan, hOverflow)) {
		widthSpan = -m_widthSpan;
		heightSpan = m_heightSpan;
		start = m_frontLLF + m_depthSpan;
	}
	// left
	else if (intersectionSegmentQuad(camToBoxCenter, m_frontLLF + m_depthSpan, m_frontLLF + m_depthSpan + m_heightSpan, m_frontLLF, m_frontLLF + m_heightSpan, hOverflow)) {
		widthSpan = glm::cross(m_widthSpan, m_heightSpan);
		heightSpan = m_heightSpan;
		start = m_frontLLF;
	}
	// right
	else if (intersectionSegmentQuad(camToBoxCenter, m_frontLLF + m_widthSpan, m_frontLLF + m_widthSpan + m_heightSpan, m_frontLLF + m_widthSpan + m_depthSpan, m_frontLLF + m_widthSpan + m_depthSpan + m_heightSpan, hOverflow)) {
		widthSpan = -glm::cross(m_widthSpan, m_heightSpan);
		heightSpan = m_heightSpan;
		start = m_frontLLF + m_widthSpan + m_depthSpan;
	}
	else {
		// no intersection - skip positioning update
		return;
	}

	glm::mat4 rotationMat = glm::rotate(glm::mat4(1), hOverflow * MAX_ROTATION_ANGLE, heightSpan);

	widthSpan = glm::normalize(glm::vec3(rotationMat * glm::vec4(widthSpan, 1.f))) * glm::length(m_widthSpan);
	heightSpan = glm::vec3(rotationMat * glm::vec4(heightSpan, 1.f));

	// bring annotation forward depending on the angular rotation to avoid occlusion
	state.setExtends(start, start + widthSpan, start + widthSpan + heightSpan);
	reinterpret_cast<glannotations::Rect*>(m_externalPrimitive.get())->setPosition(state.getLL(), state.getLR(), state.getUR());
}

void glannotations::FlagReference::updatePositioning(PathState& state) {
	throw std::logic_error("The method or operation is not implemented.");
}

glannotations::FlagReference::FlagReference(float widthOffset, glm::vec3 depthSpan, bool onlyPositioning /* = true */)
	: glannotations::AbstractExternalReference(onlyPositioning) {
	m_depthSpan = depthSpan;
}

void glannotations::FlagReference::draw() {
	gl::glDepthMask(gl::GL_FALSE);
	AbstractExternalReference::draw();
	gl::glDepthMask(gl::GL_TRUE);
}

void glannotations::FlagReference::setupExternalReference(const InternalState& state) {
	AbstractExternalReference::setupExternalReference(state);
	m_frontLLF = state.getLL();
	m_widthSpan = state.getLR() - state.getLL();
	m_heightSpan = state.getUR() - state.getLR();

	if (isDirty() && !isPositioningOnly()) {
		auto flag = new glannotations::Rect;
		flag->setPosition(m_frontLLF, m_frontLLF + m_widthSpan, m_frontLLF + m_widthSpan + m_heightSpan);
		m_externalPrimitive = flag;
	}
}

void glannotations::FlagReference::setupExternalReference(const PathState& state) {
	AbstractExternalReference::setupExternalReference(state);

	throw std::logic_error("The method or operation is not implemented.");
}

bool glannotations::FlagReference::intersectionSegmentQuad(const glannotations::Utility::Segment& ray, const glm::vec3& a, const glm::vec3& b, const glm::vec3& c, const glm::vec3& d, float& hOverflow) {
	glm::vec3 point;
	if (ray.intersect(Utility::Triangle(a, b, c), point) || ray.intersect(Utility::Triangle(c, b, d), point)) {
		hOverflow = calculateOverflow(a, c, point, glm::length(m_widthSpan) / 2.0f);
		return true;
	}
	return false;
}

float glannotations::FlagReference::calculateOverflow(const glm::vec3& a, const glm::vec3& b, const glm::vec3& point, float overflowLimit) {
	glm::vec3 projectedPoint;
	Utility::Segment segment(a, b);
	segment.orthographicProjection(point, projectedPoint);

	// check both distances to endpoints
	float distance1 = glm::distance(projectedPoint, a) - overflowLimit;
	float distance2 = glm::distance(projectedPoint, b) - overflowLimit;

	if (distance1 > 0.f && distance2 > 0.f) {
		return 0.f;
	}
	if (distance1 < distance2) {
		return distance1 / overflowLimit;
	}
	return -distance2 / overflowLimit;
}