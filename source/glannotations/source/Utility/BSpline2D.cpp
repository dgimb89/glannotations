#include <glannotations/Utility/BSpline2D.h>

#include <glm/gtx/vector_angle.hpp>
#include <glm/gtx/rotate_vector.hpp>

#include <iostream>
#include <math.h>

glannotations::BSpline2D::BSpline2D(glm::vec3 planeAxisDirection, glm::vec3 planeAxisUp, std::vector<glm::vec2> ctrlPoints, std::vector<float> knotValues)
: BSpline(knotValues) {
	assert(ctrlPoints.size() > 0);
	setPlane(planeAxisDirection, planeAxisUp);
	
	m_ctrlPoints2D = ctrlPoints;
	updateControlPoints3D();
		
	calculateSplineDegree();
	calculateArcLengths();
}

glannotations::BSpline2D::BSpline2D(glm::vec3 planeAxisDirection, glm::vec3 planeAxisUp, std::vector<glm::vec2> ctrlPoints, unsigned short degree)
: BSpline(degree) {
	assert(ctrlPoints.size() > 0);
	setPlane(planeAxisDirection, planeAxisUp);
	
	m_ctrlPoints2D = ctrlPoints;
	updateControlPoints3D();

	calculateUniformKnotValues();
	calculateArcLengths();
}

const std::vector<glm::vec2>& glannotations::BSpline2D::getControlPoints2D() const {
	return m_ctrlPoints2D;
}

glm::vec3 glannotations::BSpline2D::getPlaneNormal() const {
	return glm::cross(m_direction, m_up);
}

glm::vec3 glannotations::BSpline2D::getPlaneDirection() const {
	return m_direction;
}

glm::vec3 glannotations::BSpline2D::getPlaneUp() const {
	return m_up;
}

void glannotations::BSpline2D::setPlane(glm::vec3 planeAxisDirection, glm::vec3 planeAxisUp) {
	setDirty(true);
	m_transformationDirty = true;

	m_width = glm::length(planeAxisDirection);
	m_height = glm::length(planeAxisUp);

	m_direction = glm::normalize(planeAxisDirection);
	m_up = glm::normalize(planeAxisUp);

	float dot = glm::dot(m_direction, m_up);
	if (dot >= std::numeric_limits<float>::epsilon()) {
		std::cerr << "#Warning: planeAxisDirection is not orthogonal to planeAxisUp. Behaviour undefined.\n";
	}

	updateControlPoints3D();
}

void glannotations::BSpline2D::setControlPoints(std::vector<glm::vec2> ctrlPoints) {
	assert(ctrlPoints.size() > 0);
	setDirty(true);

	m_ctrlPoints2D = ctrlPoints;
	
	updateControlPoints3D();

	calculateArcLengths();
}


glm::vec4 glannotations::BSpline2D::getBoundingRect() {
	//todo: maybe there is a more efficient way

	float maxX = std::numeric_limits<float>::min();
	float maxY = std::numeric_limits<float>::min();
	float minX = std::numeric_limits<float>::max();
	float minY = std::numeric_limits<float>::max();
	
	for (auto ctrlPoint : m_ctrlPoints2D) {
		maxX = std::fmaxf(maxX, ctrlPoint.x);
		maxY = std::fmaxf(maxY, ctrlPoint.y);

		minX = std::fminf(minX, ctrlPoint.x);
		minY = std::fminf(minY, ctrlPoint.y);
	}

	return glm::vec4(minX, minY, maxX, maxY);
}

glm::mat4 glannotations::BSpline2D::retrieveTransformation() {
	if (!m_transformationDirty) {
		return m_transformation;
	}

	//no translation, because SplineState will take care of positioning later

	//scaling matrix
	glm::mat4 scalingM = glm::mat4();

	glm::vec4 plane = getBoundingRect();
	float scaleX = m_width / (plane.z - plane.x);
	float scaleY = m_height / (plane.w - plane.y);
	scalingM = glm::scale(scalingM, glm::vec3(scaleX, scaleY, 1.f));
	
	//rotation matrix
	glm::mat4 rotationM = glm::mat4();

	float pi = glm::pi<float>();

	//initially, BSpline2D is defined in x-y-plane
	glm::vec3 xAxis(1, 0, 0);
	glm::vec3 yAxis(0, 1, 0);

	glm::vec3 axisDirection = glm::normalize(glm::cross(xAxis, m_direction));
	if (glm::any(glm::isnan(axisDirection))) {
		//means that m_direction == xAxis (angle 0�) -> no rotation at all, rotation vector could be anything.
		//or m_direction == -xAxis (angle 180�) -> use any rotation vector orthogonal to xAxis.
		axisDirection = yAxis;
	}
	float angleDirection = glm::orientedAngle(xAxis, m_direction, axisDirection);

	if (std::abs(angleDirection) >= std::numeric_limits<float>::epsilon()) {
		//angle !== 0
		rotationM = glm::rotate(rotationM, angleDirection, axisDirection);
	}

	glm::vec4 transformedDirection = glm::vec4(xAxis.x, xAxis.y, xAxis.z, 1.0f);
	transformedDirection = rotationM * transformedDirection;
	glm::vec3 transformedDirection3D = glm::normalize(glm::vec3(transformedDirection));

	glm::vec4 transformedUp = glm::vec4(yAxis.x, yAxis.y, yAxis.z, 1.0f);
	transformedUp = rotationM * transformedUp;
	glm::vec3 transformedUp3D = glm::normalize(glm::vec3(transformedUp));

	glm::vec3 axisUp = glm::normalize(transformedDirection3D);
	float angleUp = glm::orientedAngle(transformedUp3D, m_up, axisUp);

	glm::mat4 rotationM2 = glm::mat4();

	if (std::abs(angleUp) >= std::numeric_limits<float>::epsilon()) {
		//angle !== 0
		glm::vec3 axisUp;
		axisUp = glm::normalize(transformedDirection3D);
		rotationM2 = glm::rotate(glm::mat4(), angleUp, axisUp);
	}

	m_transformation = rotationM2 * rotationM * scalingM;
	m_transformationDirty = false;

	return m_transformation;
}

void glannotations::BSpline2D::updateControlPoints3D() {
	if (m_ctrlPoints2D.size() < 1) {
		//because of initializing order in constructor
		return;
	}

	m_ctrlPoints.clear();

	//calculate ctrlpoints using plane defined by m_direction and m_up
	std::vector<glm::vec4> transformedCtrlPoints;
	for (size_t i = 0; i < m_ctrlPoints2D.size(); i++) {
		glm::vec4 transformedPoint = glm::vec4(m_ctrlPoints2D[i].x, m_ctrlPoints2D[i].y, 0, 1);
		transformedCtrlPoints.push_back(transformedPoint);
	}

	auto transformation = retrieveTransformation();

	//transform all points with calculated transformationMatrix, discard w
	for (size_t i = 0; i < transformedCtrlPoints.size(); i++) {
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
