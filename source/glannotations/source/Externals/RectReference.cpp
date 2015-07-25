#define _USE_MATH_DEFINES
#include <cmath>
#include <stdexcept>
#include <iostream>

#include <glannotations/Externals/RectReference.h>
#include <glannotations/States/SplineState.h>
#include <glannotations/States/QuadState.h>
#include <glannotations/Common/globals.h>
#include <iostream>


glannotations::RectReference::RectReference()
	: glannotations::AbstractExternalReference(true) {
}

void glannotations::RectReference::setupExternalReference(const QuadState& state) {
	AbstractExternalReference::setupExternalReference(state);
	m_widthSpan = glm::normalize(state.getLR() - state.getLL());
	m_heightSpan = glm::normalize(state.getUR() - state.getLR());
	m_center = (state.getLL() + state.getUR()) * 0.5f;
	m_up = glm::cross(m_widthSpan, m_heightSpan);

	m_halfAnnotWidth = glm::distance(state.getLR(), state.getLL()) / 2.f;
	m_halfAnnotHeight = glm::distance(state.getUR(), state.getLR()) / 2.f;
}

void glannotations::RectReference::setupExternalReference(const SplineState& state) {
	AbstractExternalReference::setupExternalReference(state);
	m_widthSpan = glm::normalize(state.getLR() - state.getLL());
	m_heightSpan = glm::normalize(state.getUR() - state.getLR());
	m_center = (state.getLL() + state.getUR()) * 0.5f;
	m_up = glm::cross(m_widthSpan, m_heightSpan);

	m_halfAnnotWidth = glm::distance(state.getLR(), state.getLL()) / 2.f;
	m_halfAnnotHeight = glm::distance(state.getUR(), state.getLR()) / 2.f;
}

void glannotations::RectReference::updatePositioning(QuadState& state) {
	glm::vec3 cameraRight = glannotations::getRight(getBindingIndex());
	glm::vec3 widthSpan, heightSpan;
	glm::vec3 n = glm::normalize(glm::cross(false ? -m_widthSpan : m_widthSpan, false ? -m_heightSpan : m_heightSpan));
	cameraRight = glm::normalize(cameraRight - glm::dot(cameraRight, n) * n);
	float widthDot = glm::dot(cameraRight, false ? -m_widthSpan : m_widthSpan);
	float depthDot = glm::dot(cameraRight, false ? -m_heightSpan : m_heightSpan);
	if (std::fabs(widthDot) > std::fabs(depthDot)) {
		widthSpan = std::signbit(widthDot) ^ false ? -m_widthSpan : m_widthSpan;
		heightSpan = glm::normalize(glm::cross(widthSpan, false ? m_up : -m_up));
		widthSpan *= m_halfAnnotWidth;
		heightSpan *= m_halfAnnotHeight;
	} else {
		widthSpan = std::signbit(depthDot) ^ false ? -m_heightSpan : m_heightSpan;
		heightSpan = glm::normalize(glm::cross(widthSpan, false ? m_up : -m_up));
		widthSpan *= m_halfAnnotHeight;
		heightSpan *= m_halfAnnotWidth;
	}
	state.setExtends(m_center - widthSpan - heightSpan, m_center + widthSpan - heightSpan, m_center + widthSpan + heightSpan);
}

void glannotations::RectReference::updatePositioning(SplineState& state) {
	glm::vec3 cameraRight = glannotations::getRight(getBindingIndex());
	glm::vec3 widthSpan, heightSpan;
	glm::vec3 n = glm::normalize(glm::cross(false ? -m_widthSpan : m_widthSpan, false ? -m_heightSpan : m_heightSpan));
	cameraRight = glm::normalize(cameraRight - glm::dot(cameraRight, n) * n);
	float widthDot = glm::dot(cameraRight, false ? -m_widthSpan : m_widthSpan);
	float depthDot = glm::dot(cameraRight, false ? -m_heightSpan : m_heightSpan);
	if (std::fabs(widthDot) > std::fabs(depthDot)) {
		widthSpan = std::signbit(widthDot) ^ false ? -m_widthSpan : m_widthSpan;
		heightSpan = glm::normalize(glm::cross(widthSpan, false ? m_up : -m_up));
		widthSpan *= m_halfAnnotWidth;
		heightSpan *= m_halfAnnotHeight;
	}
	else {
		widthSpan = std::signbit(depthDot) ^ false ? -m_heightSpan : m_heightSpan;
		heightSpan = glm::normalize(glm::cross(widthSpan, false ? m_up : -m_up));
		widthSpan *= m_halfAnnotHeight;
		heightSpan *= m_halfAnnotWidth;
	}
	state.setExtends(m_center - widthSpan - heightSpan, m_center + widthSpan - heightSpan, m_center + widthSpan + heightSpan);
}
