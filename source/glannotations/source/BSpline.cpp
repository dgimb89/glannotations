#include <glannotations/BSpline.h>
#include <iostream> //debug todo: remove

const std::vector<glm::vec3>& glannotations::BSpline::getControlPoints() {
	return m_ctrlPoints;
}

const std::vector<float>& glannotations::BSpline::getKnotValues() {
	return m_knotValues;
}

unsigned short glannotations::BSpline::getSplineDegree() {
	return m_degree;
}

void glannotations::BSpline::setKnotValues(std::initializer_list<float> knotValues) {
	setDirty(true);
	m_knotValues = knotValues;
}

void glannotations::BSpline::setControlPoints(std::initializer_list<glm::vec3> ctrlPoints) {
	setDirty(true);
	m_ctrlPoints = ctrlPoints;
}

glannotations::BSpline::BSpline(std::initializer_list<glm::vec3> ctrlPoints, std::initializer_list<float> knotValues) {
	setControlPoints(ctrlPoints);
	setKnotValues(knotValues);
	calculateSplineDegree();
}

glannotations::BSpline::BSpline(std::initializer_list<glm::vec3> ctrlPoints, unsigned short degree) {
	setControlPoints(ctrlPoints);
	m_degree = degree;
	calculateUniformKnotValues();
}

void glannotations::BSpline::calculateUniformKnotValues() {
	m_knotValues.clear();

	// starting knot -- nonperiodic B-Spline
	m_knotValues.insert(m_knotValues.end(), m_degree, 0.f);

	unsigned ctrlCount = m_ctrlPoints.size();
	float uniformDistance = 1.0f / ctrlCount;
	float current = 0.f;
	// internal knots - uniform distribution
	for (unsigned n = 0; n < ctrlCount; ++n) {
		m_knotValues.push_back(current);
		current += uniformDistance;
	}
	// ending knot
	m_knotValues.insert(m_knotValues.end(), m_degree, 1.f);

	setDirty(true);
}

void glannotations::BSpline::calculateSplineDegree() {
	m_degree = m_knotValues.size() - m_ctrlPoints.size() - 1;
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
	int numberOfDivisions = 10 * m_ctrlPoints.size();
	int maxPoint = numberOfDivisions + 1;

	glm::vec3 previousPoint = getCurvepointAt(0);

	float sum = 0;

	m_arcLengths.clear();
	m_arcLengths.push_back(0.0);

	for (int i = 1; i <= maxPoint; i++){
		glm::vec3 p = getCurvepointAt(i / maxPoint);

		//calculate distance
		glm::vec3 temp = previousPoint - p;
		float distSqr = dot(temp, temp);

		sum += distSqr;

		previousPoint = p;
		m_arcLengths.push_back(sum);
	};
};

glm::vec3 glannotations::BSpline::getCurvepointAt(float t) {
	if ( t < 0.0f || t > 1.0f)
		std::cout << "debug: warning! 0 <= t <= 1";

	float tempT = t* m_ctrlPoints.size() - 1;
	int i = std::floor(tempT) + m_degree - 1;
	glm::vec3 point = deBoor(m_degree, m_degree, i, tempT, m_knotValues);

	return point;
};

//not sure if the use of "inline" is correct here
inline float clamp(float value, float min, float max){
	//because there is no clamp in std
	return value < min ? min : (value > max ? max : value);
};

glm::vec3 glannotations::BSpline::deBoor(int k, int degree, int i, float t, std::vector<float> knots) {
	if (k == 0) {
		i = clamp(i, 0.0f, m_ctrlPoints.size() - 1);
		return m_ctrlPoints[i];
	}
	else {
		float alpha;
		try {
			alpha = (t - knots[i]) / (knots[i + degree + 1 - k] - knots[i]);
		} catch (...) {
			alpha = 1.0f;
			std::cout << "debug: alpha problem";
		}

		return (deBoor(k - 1, degree, i - 1, t, knots) * (1.0f - alpha) + (deBoor(k - 1, degree, i, t, knots) * (alpha)));
	}
};

glm::vec2 glannotations::BSpline::getSecantVectorAt(float t, float nextT) {
	glm::vec3 currentPoint = getCurvepointAt(t);
	glm::vec3 nextPoint = getCurvepointAt(nextT);
	glm::vec2 secantVector(nextPoint - currentPoint);

	return secantVector;
};