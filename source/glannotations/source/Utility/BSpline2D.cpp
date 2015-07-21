#include <glannotations/Utility/BSpline2D.h>

#include <glm/gtx/vector_angle.hpp>
#include <glm/gtx/rotate_vector.hpp>

#include <iostream>
#include <math.h>

glannotations::BSpline2D::BSpline2D(std::vector<glm::vec2> ctrlPoints, std::vector<float> knotValues)
: BSpline(knotValues) {
	assert(ctrlPoints.size() > 0);
	
	setControlPoints(ctrlPoints);
		
	calculateSplineDegree();
	calculateArcLengths();
}

glannotations::BSpline2D::BSpline2D(std::vector<glm::vec2> ctrlPoints, unsigned short degree)
: BSpline(degree) {
	assert(ctrlPoints.size() > 0);
	
	setControlPoints(ctrlPoints);

	calculateUniformKnotValues();
	calculateArcLengths();
}

void glannotations::BSpline2D::setControlPoints(std::vector<glm::vec2> ctrlPoints) {
	assert(ctrlPoints.size() > 0);
	setDirty(true);

	m_ctrlPoints.clear();

	for (int i = 0; i < ctrlPoints.size(); i++) {
		m_ctrlPoints.push_back(glm::vec3(ctrlPoints[i].x, ctrlPoints[i].y, 0.f));
	}
	
	calculateArcLengths();
}


glm::vec4 glannotations::BSpline2D::getBoundingRect() {
	//todo: maybe there is a more efficient way

	float maxX = std::numeric_limits<float>::min();
	float maxY = std::numeric_limits<float>::min();
	float minX = std::numeric_limits<float>::max();
	float minY = std::numeric_limits<float>::max();
	
	for (auto ctrlPoint : m_ctrlPoints) {
		maxX = std::fmaxf(maxX, ctrlPoint.x);
		maxY = std::fmaxf(maxY, ctrlPoint.y);

		minX = std::fminf(minX, ctrlPoint.x);
		minY = std::fminf(minY, ctrlPoint.y);
	}

	return glm::vec4(minX, minY, maxX, maxY);
}