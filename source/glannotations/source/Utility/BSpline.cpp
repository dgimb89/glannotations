#include <glannotations/Utility/BSpline.h>

#include <glm/gtx/vector_angle.hpp>
#include <glm/gtx/rotate_vector.hpp>

#include <iostream> //debug todo:anne remove

glannotations::BSpline::BSpline(std::vector<glm::vec3> ctrlPoints, std::vector<float> knotValues) {
	assert(ctrlPoints.size() > 0);
	assert(knotValues.size() > 0);

	setControlPoints(ctrlPoints);
	setKnotValues(knotValues);
	calculateSplineDegree();
	calculateArcLengths();
}

glannotations::BSpline::BSpline(std::vector<glm::vec3> ctrlPoints, unsigned short degree) {
	assert(ctrlPoints.size() > 0);
	assert(degree > 0);

	setControlPoints(ctrlPoints);
	m_degree = degree;
	calculateUniformKnotValues();
	calculateArcLengths();
}

/*
glannotations::BSpline::BSpline(std::vector<glm::vec2> ctrlPoints, std::vector<float> knotValues, glm::vec3 planeNormal, glm::vec3 firstControlPointOnTargetPlane, glm::vec3 lastControlPointOnTargetPlane){
	project2DContropointsToPlane(ctrlPoints, planeNormal, firstControlPointOnTargetPlane, lastControlPointOnTargetPlane);

	setKnotValues(knotValues);
	calculateSplineDegree();
	calculateArcLengths();
}
glannotations::BSpline::BSpline(std::vector<glm::vec2> ctrlPoints, unsigned short degree, glm::vec3 planeNormal, glm::vec3 firstControlPointOnTargetPlane, glm::vec3 lastControlPointOnTargetPlane){
	project2DContropointsToPlane(ctrlPoints, planeNormal, firstControlPointOnTargetPlane, lastControlPointOnTargetPlane);

	m_degree = degree;
	calculateUniformKnotValues();
	calculateArcLengths();
}

void glannotations::BSpline::project2DContropointsToPlane(std::vector<glm::vec2> ctrlPoints, glm::vec3 planeNormal, glm::vec3 firstControlPointOnTargetPlane, glm::vec3 lastControlPointOnTargetPlane){
	//todo:anne needs testing!
	setDirty(true);
	
	std::vector<glm::vec2> controlPoints2D = ctrlPoints;
	std::vector<glm::vec3> controlPoints3D;
	
	planeNormal = glm::normalize(planeNormal);

	//todo:anne maybe necessary?
	//glm::vec3 up = lastControlPointOnTargetPlane - firstControlPointOnTargetPlane;
	//float rotationAngleZAxis = glm::angle(glm::vec3(0, 1, 0), up);
	//we could use "up" for extends of spline, or something like that
	
	float rotationAngle = glm::angle(glm::vec3(0, 0, 1), planeNormal);
	glm::vec3 rotationAxis = glm::cross(glm::vec3(0, 0, 1), planeNormal);

	glm::vec3 translationVec = firstControlPointOnTargetPlane - glm::vec3(controlPoints2D.at(0), 0.f);

	//todo:anne transform points based on given plane
    for (unsigned int i = 0; i < controlPoints2D.size(); i++){
		glm::vec3 projectedPoint = glm::vec3(controlPoints2D.at(i), 0.f);

		projectedPoint = glm::rotate(projectedPoint, rotationAngle, rotationAxis);
		projectedPoint += translationVec;
		
		controlPoints3D.push_back(projectedPoint);
	}

	m_ctrlPoints = controlPoints3D;
}
*/
const std::vector<glm::vec3>& glannotations::BSpline::getControlPoints() {
	return m_ctrlPoints;
}

const std::vector<float>& glannotations::BSpline::getKnotValues() {
	return m_knotValues;
}

unsigned short glannotations::BSpline::getSplineDegree() {
	return m_degree;
}

void glannotations::BSpline::setKnotValues(std::vector<float> knotValues) {
	setDirty(true);
	m_knotValues = knotValues;
}

void glannotations::BSpline::setControlPoints(std::vector<glm::vec3> ctrlPoints) {
	setDirty(true);
	m_ctrlPoints = ctrlPoints;
}

void glannotations::BSpline::calculateUniformKnotValues() {
	m_knotValues.clear();

	// starting knots; nonperiodic B-Spline
	m_knotValues.insert(m_knotValues.end(), m_degree, 0.f);
	// todo: this should be garantied
	assert(m_ctrlPoints.size() > 0);
	size_t ctrlCount = m_ctrlPoints.size() - 1;
	
	//internal knots - uniform distribution
	for (unsigned n = 1; n < ctrlCount; ++n) {
		m_knotValues.push_back(static_cast<float>(n));
	}
	// ending knots
	m_knotValues.insert(m_knotValues.end(), m_degree, static_cast<float>(ctrlCount));

	setDirty(true);
}

void glannotations::BSpline::calculateSplineDegree() {
	m_degree = static_cast<unsigned short>(m_knotValues.size() - m_ctrlPoints.size() - 1);
	setDirty(true);
}

void glannotations::BSpline::calculateArcLengths() {
	//It may be possible to estimate the error and dynamically pick an "ideal" number of divisions.
	//This could be done by subdividing until segment lengths are below some threshold or
	//by comparing the tangents of the line segments to the tangents of the curve,
	//but I have not explored either of these ideas.
	//Fixing the number of divisions at around 100 seems to work well enough for the purpose at hand. 
	//UPDATE: the longer the curve, the higher numberOfSubdivisions needed
	//& the longer an icicle, the more curve points given
	// => the more curve points, the higher numberOfSubdivisions!
	size_t numberOfDivisions = 10 * m_ctrlPoints.size();
	size_t maxPoint = numberOfDivisions + 1;

	glm::vec3 previousPoint = retrieveCurvepointAt(0);

	float sum = 0;

	m_arcLengths.clear();
	m_arcLengths.push_back(0.0);

    for (unsigned int i = 1; i < maxPoint; i++){
        glm::vec3 p = retrieveCurvepointAt(static_cast<float>(i) / static_cast<float>(maxPoint));
		sum += glm::distance(p, previousPoint);
		previousPoint = p;
		m_arcLengths.push_back(sum);
	};

	glm::vec3 p = retrieveCurvepointAt(1.f);
	sum += glm::distance(p, previousPoint);
	previousPoint = p;
	m_arcLengths.push_back(sum);
};

glm::vec3 glannotations::BSpline::retrieveCurvepointAt(float t) {
	if ( t < 0.0f || t > 1.0f)
		std::cout << "debug: warning! 0 <= t <= 1";

    float tempT = t * static_cast<float>(m_ctrlPoints.size() - 1);
	int i = static_cast<int>(std::floor(tempT)) + m_degree - 1;
	glm::vec3 point = deBoor(m_degree, m_degree, i, tempT, m_knotValues);

	return point;
};

inline int binaryIndexOfLargestValueSmallerThanOrEqualTo(std::vector<float> container, float searchElement){
    unsigned int minIndex = 0;
	assert(container.size() > 0);
	size_t maxIndex = container.size() - 1;
    unsigned int currentIndex;
	float currentElement;

	while (minIndex <= maxIndex) {
        currentIndex = static_cast<unsigned int>(std::floor((minIndex + maxIndex) / 2));
		currentElement = container.at(currentIndex);

        if (std::abs(currentElement - searchElement) < std::numeric_limits<float>::epsilon())
			return currentIndex;
		if (currentElement >= searchElement &&
			currentIndex == 0)
			return currentIndex;
		if (currentElement >= searchElement &&
			(container.at(currentIndex - 1) < searchElement))
			return currentIndex - 1;
		if (currentElement <= searchElement &&
			(currentIndex >= container.size() || container.at(currentIndex + 1) > searchElement))
			return currentIndex;

		if (currentElement < searchElement)
			minIndex = currentIndex + 1;
		else if (currentElement > searchElement)
			maxIndex = currentIndex - 1;
		else
			return currentIndex;
	}

	return -1;
};

glm::vec3 glannotations::BSpline::deBoor(int k, int degree, int i, float t, std::vector<float> knots) {
	if (k == 0) {
		i = clamp(i, 0, static_cast<int>(m_ctrlPoints.size() - 1));
		return m_ctrlPoints[i];
	}
	else {
		float alpha;
		try {
            if (i + degree + 1 - k >= static_cast<int>(knots.size()) || std::abs(knots[i + degree + 1 - k] - knots[i]) < std::numeric_limits<float>::epsilon())
				alpha = 1.0f;
			else
				alpha = (t - knots[i]) / (knots[i + degree + 1 - k] - knots[i]);
		} catch (...) { //this catch doesn't work, what kind of sorcery is this?
			alpha = 1.0f;
			std::cout << "debug: alpha problem";
		}

		return (deBoor(k - 1, degree, i - 1, t, knots) * (1.0f - alpha) + (deBoor(k - 1, degree, i, t, knots) * (alpha)));
	}
};

float glannotations::BSpline::getTForU(float u){
	if (u < 0 || u>1)
		std::cout << "u out of range: " << u;
	float t;
	float targetArcLength = u * m_arcLengths.back();
	int index = binaryIndexOfLargestValueSmallerThanOrEqualTo(m_arcLengths, targetArcLength);

    if (std::abs(m_arcLengths[index] - targetArcLength) < std::numeric_limits<float>::epsilon()){
		// if exact match, return t based on exact index
        t = static_cast<float>(index) / static_cast<float>(m_arcLengths.size() - 1);
	}
	else{
		// need to interpolate between two points
		float lengthBefore = m_arcLengths[index];
		float lengthAfter = m_arcLengths[index + 1];
		float segmentLength = lengthAfter - lengthBefore;
		float segmentFraction = (targetArcLength - lengthBefore) / segmentLength;
        t = (static_cast<float>(index) + segmentFraction) / static_cast<float>(m_arcLengths.size() - 1);
	}
	if (t<0 || t>1)
		std::cout << "t out of range: " << t;
	return t;
};

glm::vec3 glannotations::BSpline::retrieveSecantVectorAt(float t, float nextT) {
	glm::vec3 currentPoint = retrieveCurvepointAt(getTForU(t));
	glm::vec3 nextPoint = retrieveCurvepointAt(getTForU(nextT));
	glm::vec3 secantVector(nextPoint - currentPoint);

	return secantVector;
};
