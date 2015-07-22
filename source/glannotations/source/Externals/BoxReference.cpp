﻿#define _USE_MATH_DEFINES
#include <cmath>
#include <glbinding/gl/boolean.h>
#include <glbinding/gl/functions.h>
#include <algorithm>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

#include <glannotations/Externals/BoxReference.h>
#include <glannotations/Renderer/Box.h>
#include <glannotations/AbstractAnnotation.h>
#include <glannotations/States/SplineState.h>
#include <glannotations/States/QuadState.h>
#include <glannotations/Utility/Segment.h>
#include <glannotations/Utility/Triangle.h>

const float MAX_ROTATION_ANGLE = static_cast<float>(M_PI * 45.f / 180.f);
const float ANTI_FLICKERING = 0.01f;

void glannotations::BoxReference::updatePositioning(QuadState& state) {
	Utility::Segment camToBoxCenter(glannotations::getEye(getBindingIndex()), m_frontLLF + (m_widthSpan + m_heightSpan + m_depthSpan) / 2.f);
	glm::vec3 intersection, widthSpan, heightSpan;
	float vOverflow, hOverflow;

	//front
	if (intersectionSegmentQuad(camToBoxCenter, m_frontLLF, m_frontLLF + m_heightSpan, m_frontLLF + m_widthSpan, m_frontLLF + m_widthSpan + m_heightSpan, intersection, vOverflow, hOverflow)){
		widthSpan = m_widthSpan;
		heightSpan = m_heightSpan;
	}
	// back
	else if (intersectionSegmentQuad(camToBoxCenter, m_frontLLF + m_depthSpan + m_widthSpan, m_frontLLF + m_depthSpan + m_widthSpan + m_heightSpan, m_frontLLF + m_depthSpan, m_frontLLF + m_depthSpan + m_heightSpan, intersection, vOverflow, hOverflow)) {
		widthSpan = -m_widthSpan;
		heightSpan = m_heightSpan;
	}
	// left
	else if (intersectionSegmentQuad(camToBoxCenter, m_frontLLF + m_depthSpan, m_frontLLF + m_depthSpan + m_heightSpan, m_frontLLF, m_frontLLF + m_heightSpan, intersection, vOverflow, hOverflow)) {
		widthSpan = glm::cross(m_widthSpan, m_heightSpan);
		heightSpan = m_heightSpan;
	}
	// right
	else if (intersectionSegmentQuad(camToBoxCenter, m_frontLLF + m_widthSpan, m_frontLLF + m_widthSpan + m_heightSpan, m_frontLLF + m_widthSpan + m_depthSpan, m_frontLLF + m_widthSpan + m_depthSpan + m_heightSpan, intersection, vOverflow, hOverflow)) {
		widthSpan = -glm::cross(m_widthSpan, m_heightSpan);
		heightSpan = m_heightSpan;
	}
	// top
	else if (intersectionSegmentQuad(camToBoxCenter, m_frontLLF + m_heightSpan, m_frontLLF + m_heightSpan + m_depthSpan, m_frontLLF + m_heightSpan + m_widthSpan, m_frontLLF + m_heightSpan + m_widthSpan + m_depthSpan, intersection, vOverflow, hOverflow)) {
		determineViewdependantSpans(widthSpan, heightSpan, false, vOverflow, hOverflow);
	}
	// bottom
	else if (intersectionSegmentQuad(camToBoxCenter, m_frontLLF + m_depthSpan, m_frontLLF, m_frontLLF + m_widthSpan + m_depthSpan, m_frontLLF + m_widthSpan, intersection, vOverflow, hOverflow)) {
		determineViewdependantSpans(widthSpan, heightSpan, true, vOverflow, hOverflow);
	}
	else {
		// no intersection - skip positioning update
		return;
	}

	widthSpan = glm::normalize(widthSpan) * m_halfAnnotWidth;
	heightSpan = glm::normalize(heightSpan) * m_halfAnnotHeight;

	glm::mat4 rotationMat = glm::rotate(glm::rotate(glm::mat4(1), hOverflow * MAX_ROTATION_ANGLE, heightSpan), vOverflow * -MAX_ROTATION_ANGLE, widthSpan);

	widthSpan = glm::vec3(rotationMat * glm::vec4(widthSpan, 1.f));
	heightSpan = glm::vec3(rotationMat * glm::vec4(heightSpan, 1.f));

	// bring annotation forward depending on the angular rotation to avoid occlusion
	intersection += glm::cross(widthSpan, heightSpan) * 0.5f * std::sqrt(std::sqrt(std::max(std::fabs(vOverflow), std::fabs(hOverflow))));
	state.setExtends(intersection - widthSpan - heightSpan, intersection + widthSpan - heightSpan, intersection + widthSpan + heightSpan);
}

void glannotations::BoxReference::updatePositioning(SplineState& state) {
	Utility::Segment camToBoxCenter(glannotations::getEye(getBindingIndex()), m_frontLLF + (m_widthSpan + m_heightSpan + m_depthSpan) / 2.f);
	glm::vec3 intersection, widthSpan, heightSpan;
	float vOverflow, hOverflow;

	//front
	if (intersectionSegmentQuad(camToBoxCenter, m_frontLLF, m_frontLLF + m_heightSpan, m_frontLLF + m_widthSpan, m_frontLLF + m_widthSpan + m_heightSpan, intersection, vOverflow, hOverflow)){
		widthSpan = m_widthSpan;
		heightSpan = m_heightSpan;
	}
	// back
	else if (intersectionSegmentQuad(camToBoxCenter, m_frontLLF + m_depthSpan + m_widthSpan, m_frontLLF + m_depthSpan + m_widthSpan + m_heightSpan, m_frontLLF + m_depthSpan, m_frontLLF + m_depthSpan + m_heightSpan, intersection, vOverflow, hOverflow)) {
		widthSpan = -m_widthSpan;
		heightSpan = m_heightSpan;
	}
	// left
	else if (intersectionSegmentQuad(camToBoxCenter, m_frontLLF + m_depthSpan, m_frontLLF + m_depthSpan + m_heightSpan, m_frontLLF, m_frontLLF + m_heightSpan, intersection, vOverflow, hOverflow)) {
		widthSpan = glm::cross(m_widthSpan, m_heightSpan);
		heightSpan = m_heightSpan;
	}
	// right
	else if (intersectionSegmentQuad(camToBoxCenter, m_frontLLF + m_widthSpan, m_frontLLF + m_widthSpan + m_heightSpan, m_frontLLF + m_widthSpan + m_depthSpan, m_frontLLF + m_widthSpan + m_depthSpan + m_heightSpan, intersection, vOverflow, hOverflow)) {
		widthSpan = -glm::cross(m_widthSpan, m_heightSpan);
		heightSpan = m_heightSpan;
	}
	// top
	else if (intersectionSegmentQuad(camToBoxCenter, m_frontLLF + m_heightSpan, m_frontLLF + m_heightSpan + m_depthSpan, m_frontLLF + m_heightSpan + m_widthSpan, m_frontLLF + m_heightSpan + m_widthSpan + m_depthSpan, intersection, vOverflow, hOverflow)) {
		determineViewdependantSpans(widthSpan, heightSpan, false, vOverflow, hOverflow);
	}
	// bottom
	else if (intersectionSegmentQuad(camToBoxCenter, m_frontLLF + m_depthSpan, m_frontLLF, m_frontLLF + m_widthSpan + m_depthSpan, m_frontLLF + m_widthSpan, intersection, vOverflow, hOverflow)) {
		determineViewdependantSpans(widthSpan, heightSpan, true, vOverflow, hOverflow);
	}
	else {
		// no intersection - skip positioning update
		return;
	}

	widthSpan = glm::normalize(widthSpan) * m_halfAnnotWidth;
	heightSpan = glm::normalize(heightSpan) * m_halfAnnotHeight;

	glm::mat4 rotationMat = glm::rotate(glm::rotate(glm::mat4(1), hOverflow * MAX_ROTATION_ANGLE, heightSpan), vOverflow * -MAX_ROTATION_ANGLE, widthSpan);

	widthSpan = glm::vec3(rotationMat * glm::vec4(widthSpan, 1.f));
	heightSpan = glm::vec3(rotationMat * glm::vec4(heightSpan, 1.f));

	// bring annotation forward depending on the angular rotation to avoid occlusion
	intersection += glm::cross(widthSpan, heightSpan) * 0.5f * std::sqrt(std::sqrt(std::max(std::fabs(vOverflow), std::fabs(hOverflow))));
	state.setExtends(intersection - widthSpan - heightSpan, intersection + widthSpan - heightSpan, intersection + widthSpan + heightSpan);

}

glannotations::BoxReference::BoxReference(glm::vec2 widthOverflow, glm::vec2 heightOverflow, glm::vec3 depthSpan, bool onlyPositioning /* = true */)
	: glannotations::AbstractExternalReference(onlyPositioning) {
	// we use internal span vectors to save overflow ranges for now
	m_widthSpan = glm::vec3(widthOverflow, 0.f);
	m_heightSpan = glm::vec3(heightOverflow, 0.f);
	m_depthSpan = depthSpan;
	fixFlickering();
}

void glannotations::BoxReference::draw() {
	gl::glDepthMask(gl::GL_FALSE);
	AbstractExternalReference::draw();
	gl::glDepthMask(gl::GL_TRUE);
}

void glannotations::BoxReference::setupExternalReference(const QuadState& state) {
	AbstractExternalReference::setupExternalReference(state);
	m_frontLLF = state.getLL() - (m_widthSpan.x * glm::normalize(state.getLR() - state.getLL())) - (m_heightSpan.x * glm::normalize(state.getUR() - state.getLR()));

	// calculate absolute span vectors from annotation state + given overflow ranges
	glm::vec3 spanVector = glm::normalize(state.getLR() - state.getLL());
	m_widthSpan = (state.getLR() + (spanVector * m_widthSpan.y)) - (state.getLL() - (spanVector * m_widthSpan.x));

	spanVector = glm::normalize(state.getUR() - state.getLR());
	m_heightSpan = (state.getUR() + (spanVector * m_heightSpan.y)) - (state.getLR() - (spanVector * m_heightSpan.x));

	m_halfAnnotWidth = glm::distance(state.getLR(), state.getLL()) / 2.f;
	m_halfAnnotHeight = glm::distance(state.getUR(), state.getLR()) / 2.f;

	if (isDirty() && !isPositioningOnly()) {
		auto box = new glannotations::Box;
		box->setPosition(m_frontLLF, m_frontLLF + m_widthSpan + m_heightSpan + m_depthSpan);
		m_externalPrimitive = box;
	}
}

void glannotations::BoxReference::setupExternalReference(const SplineState& state) {
	AbstractExternalReference::setupExternalReference(state);
	m_frontLLF = state.getLL() - (m_widthSpan.x * glm::normalize(state.getLR() - state.getLL())) - (m_heightSpan.x * glm::normalize(state.getUR() - state.getLR()));

	// calculate absolute span vectors from annotation state + given overflow ranges
	glm::vec3 spanVector = glm::normalize(state.getLR() - state.getLL());
	m_widthSpan = (state.getLR() + (spanVector * m_widthSpan.y)) - (state.getLL() - (spanVector * m_widthSpan.x));

	spanVector = glm::normalize(state.getUR() - state.getLR());
	m_heightSpan = (state.getUR() + (spanVector * m_heightSpan.y)) - (state.getLR() - (spanVector * m_heightSpan.x));

	m_halfAnnotWidth = glm::distance(state.getLR(), state.getLL()) / 2.f;
	m_halfAnnotHeight = glm::distance(state.getUR(), state.getLR()) / 2.f;

	if (isDirty() && !isPositioningOnly()) {
		auto box = new glannotations::Box;
		box->setPosition(m_frontLLF, m_frontLLF + m_widthSpan + m_heightSpan + m_depthSpan);
		m_externalPrimitive = box;
	}
}

bool glannotations::BoxReference::intersectionSegmentQuad(const glannotations::Utility::Segment& ray, const glm::vec3& a, const glm::vec3& b, const glm::vec3& c, const glm::vec3& d, glm::vec3& point, float& vOverflow, float& hOverflow) {
	if (ray.intersect(Utility::Triangle(a, b, c), point) || ray.intersect(Utility::Triangle(c, b, d), point)) {
		vOverflow = calculateOverflow(a, b, point, m_halfAnnotHeight);
		hOverflow = calculateOverflow(a, c, point, m_halfAnnotWidth);
		return true;
	}
	return false;
}

float glannotations::BoxReference::calculateOverflow(const glm::vec3& a, const glm::vec3& b, const glm::vec3& point, float overflowLimit) {
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

void glannotations::BoxReference::fixFlickering() {
	m_widthSpan.x += ANTI_FLICKERING;
	m_widthSpan.y += ANTI_FLICKERING;
	m_heightSpan.x += ANTI_FLICKERING;
	m_heightSpan.y += ANTI_FLICKERING;
	m_depthSpan *= 1.f + ANTI_FLICKERING;
}

void glannotations::BoxReference::determineViewdependantSpans(glm::vec3& widthSpan, glm::vec3& heightSpan, bool bottom, float& vOverflow, float& hOverflow) {
	glm::vec3 cameraRight = glannotations::getRight(getBindingIndex());
	// project cameraRight on top plane
	glm::vec3 n = glm::normalize(glm::cross(bottom ? -m_widthSpan : m_widthSpan, bottom ? -m_depthSpan : m_depthSpan));
	cameraRight = glm::normalize(cameraRight - glm::dot(cameraRight, n) * n);
	float widthDot = glm::dot(cameraRight, bottom ? -m_widthSpan : m_widthSpan);
	float depthDot = glm::dot(cameraRight, bottom ? -m_depthSpan : m_depthSpan);
	if (std::fabs(widthDot) > std::fabs(depthDot)) {
		widthSpan = std::signbit(widthDot) ^ bottom ? -m_widthSpan : m_widthSpan;
		heightSpan = std::signbit(depthDot) ^ bottom ? -m_depthSpan : m_depthSpan;
		vOverflow = std::signbit(widthDot) ^ bottom ? -vOverflow : vOverflow;
		hOverflow = 0.f;
	}
	else {
		widthSpan = std::signbit(depthDot) ^ bottom ? -m_depthSpan : m_depthSpan;
		heightSpan = std::signbit(widthDot) ^ bottom ? -m_widthSpan : m_widthSpan;
		vOverflow = std::signbit(depthDot) ? hOverflow : -hOverflow;
		hOverflow = 0.f;
	}
	heightSpan = glm::cross(widthSpan, bottom ? m_heightSpan : -m_heightSpan);
}
