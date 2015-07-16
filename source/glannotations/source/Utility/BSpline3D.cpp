#include <glannotations/Utility/BSpline3D.h>

#include <glm/gtx/vector_angle.hpp>
#include <glm/gtx/rotate_vector.hpp>

#include <iostream>

glannotations::BSpline3D::BSpline3D(std::vector<glm::vec3> ctrlPoints, std::vector<float> knotValues)
: BSpline(knotValues) {
	assert(ctrlPoints.size() > 0);
	setControlPoints(ctrlPoints);

	calculateSplineDegree();
	calculateArcLengths();
}

glannotations::BSpline3D::BSpline3D(std::vector<glm::vec3> ctrlPoints, unsigned short degree)
: BSpline(degree) {
	assert(ctrlPoints.size() > 0);
	setControlPoints(ctrlPoints);

	calculateUniformKnotValues();
	calculateArcLengths();
}

void glannotations::BSpline3D::setControlPoints(std::vector<glm::vec3> ctrlPoints) {
	assert(ctrlPoints.size() > 0);
	setDirty(true);
	m_ctrlPoints = ctrlPoints;
}