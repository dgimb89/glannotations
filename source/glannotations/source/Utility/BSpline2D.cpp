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

	//think again:
	//get angle and axis to rotate m_up to y-axis
	//get angle and axis to rotate m_direction to x-axis
	/*
	*	todo:anne done, but still doesn't work.
	*	given m_direction=glm::vec3(0, 0, 1) and m_up=glm::vec3(0, -1, 0),
	*	it transforms to m_up=glm::vec3(0, 0, 1) and m_direction=glm::vec3(0, -1, 0)!!!
	*/
	float pi = glm::pi<float>();

	glm::vec3 xAxis(1, 0, 0);
	glm::vec3 yAxis(0, 1, 0);
	glm::vec3 zAxis(0, 0, 1);

	float angleDirection = glm::angle(m_direction, xAxis);
	if (std::abs(angleDirection) >= std::numeric_limits<float>::epsilon()) {
		//angle !== 0
		if (std::abs(std::abs(angleDirection) - pi) >= std::numeric_limits<float>::epsilon()) {
			//angle != pi
			glm::vec3 axisDirection = glm::normalize(glm::cross(m_direction, xAxis));
			transformation = glm::rotate(transformation, angleDirection, axisDirection);
		}
		else {
			//angle == pi == 180° cross product would give zero vec
			//any rotation axis orthogonal to xAxis should do
			glm::vec3 axisDirection = yAxis;
			transformation = glm::rotate(transformation, angleDirection, axisDirection);
		}
	}
	
	glm::vec4 transformedUp = glm::vec4(m_up.x, m_up.y, m_up.z, 1.0f);
	transformedUp = transformation * transformedUp;
	glm::vec3 transformedUp3D = glm::normalize(glm::vec3(transformedUp));
	

	float angleUp = glm::angle(transformedUp3D, yAxis);
	if (std::abs(angleUp) >= std::numeric_limits<float>::epsilon()) {
		//angle !== 0
		if (std::abs(std::abs(angleUp) - pi) >= std::numeric_limits<float>::epsilon()) {
			//angle != pi
			glm::vec3 axisUp = glm::normalize(glm::cross(transformedUp3D, yAxis));
			transformation = glm::rotate(transformation, angleUp, axisUp);
		}
		else {
			//angle == pi == 180° cross product would give zero vec
			//any rotation axis orthogonal to yAxis should do
			glm::vec3 axisDirection = zAxis;
			transformation = glm::rotate(transformation, angleDirection, axisDirection);
		}
	}
	
	//no translation, because SplineState will take care of positioning later
	//no scale, because m_up and m_direction are normalized, and we don't care about size

	//transform all points with calculated transformationMatrix, discard w
	for (int i = 0; i < transformedCtrlPoints.size(); i++) {
		transformedCtrlPoints[i] = transformation * transformedCtrlPoints[i];
		m_ctrlPoints.push_back(glm::vec3(transformedCtrlPoints[i]));
	}
}

bool glannotations::BSpline2D::isInSamePlane(std::shared_ptr<BSpline2D> compareSpline) const {
	
	if (glm::distance2(compareSpline->m_direction, m_direction) >= std::numeric_limits<float>::epsilon())
		return false;

	if (glm::distance2(compareSpline->m_up, m_up) >= std::numeric_limits<float>::epsilon())
		return false;

	return true;
}