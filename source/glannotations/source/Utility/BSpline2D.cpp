#include <glannotations/Utility/BSpline2D.h>

#include <glm/gtx/vector_angle.hpp>
#include <glm/gtx/rotate_vector.hpp>

#include <iostream>

glannotations::BSpline2D::BSpline2D(glm::vec3 planeAxisX, glm::vec3 planeAxisY, std::vector<glm::vec2> ctrlPoints, std::vector<float> knotValues)
: BSpline(knotValues) {
	assert(ctrlPoints.size() > 0);
	setPlane(planeAxisX, planeAxisY);
	setControlPoints(ctrlPoints);
		
	calculateSplineDegree();
	calculateArcLengths();
}

glannotations::BSpline2D::BSpline2D(glm::vec3 planeAxisX, glm::vec3 planeAxisY, std::vector<glm::vec2> ctrlPoints, unsigned short degree)
: BSpline(degree) {
	assert(ctrlPoints.size() > 0);
	setPlane(planeAxisX, planeAxisY);
	setControlPoints(ctrlPoints);

	calculateUniformKnotValues();
	calculateArcLengths();
}

const std::vector<glm::vec2>& glannotations::BSpline2D::getControlPoints2D() const {
	return m_ctrlPoints2D;
}

const glm::vec3& glannotations::BSpline2D::getPlaneNormal() const {
	return glm::cross(m_direction, m_up);
}

void glannotations::BSpline2D::setPlane(glm::vec3 planeAxisX, glm::vec3 planeAxisY) {
	setDirty(true);

	m_direction = glm::normalize(planeAxisX);
	m_up = glm::normalize(planeAxisY);

	updateControlPoints3D();
}

void glannotations::BSpline2D::setControlPoints(std::vector<glm::vec2> ctrlPoints) {
	assert(ctrlPoints.size() > 0);
	setDirty(true);

	m_ctrlPoints2D = ctrlPoints;
	
	updateControlPoints3D();
}

void glannotations::BSpline2D::updateControlPoints3D() {
	if (m_ctrlPoints2D.size() < 1) {
		//because of initializing order in constructor
		return;
	}

	m_ctrlPoints.clear();

	//calculate ctrlpoints using plane
	glm::vec3 normal = getPlaneNormal();

	glm::mat4 transformation = glm::mat4();
	std::vector<glm::vec4> transformedCtrlPoints;

	for (int i = 0; i < m_ctrlPoints2D.size(); i++) {
		glm::vec4 transformedPoint = glm::vec4(m_ctrlPoints2D[i].x, m_ctrlPoints2D[i].y, 0, 1);
		transformedCtrlPoints.push_back(transformedPoint);
	}

	//rotation
	float angleX = glm::angle(m_up, normal);
	glm::vec3 axisX = glm::normalize(glm::cross(m_up, normal));
	glm::rotate(transformation, angleX, axisX);

	float angleY = glm::angle(m_direction, normal);
	glm::vec3 axisY = glm::normalize(glm::cross(m_direction, normal));
	glm::rotate(transformation, angleY, axisY);

	float angleZ = glm::angle(m_direction, m_up);
	glm::vec3 axisZ = glm::normalize(glm::cross(m_direction, m_up));
	glm::rotate(transformation, angleY, axisY);

	//no translation, because SplineState will take care of positioning later
	//todo: no scale?

	//transform all points, discard w
	for (int i = 0; i < transformedCtrlPoints.size(); i++) {
		transformedCtrlPoints[i] = transformation * transformedCtrlPoints[i];
		m_ctrlPoints[i] = glm::vec3(transformedCtrlPoints[i]);
	}
}

bool glannotations::BSpline2D::isInSamePlane(std::shared_ptr<BSpline2D> compareSpline) const {
	
	if (glm::distance2(compareSpline->m_direction, m_direction) >= std::numeric_limits<float>::epsilon())
		return false;

	if (glm::distance2(compareSpline->m_up, m_up) >= std::numeric_limits<float>::epsilon())
		return false;

	return true;
}