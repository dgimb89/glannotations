#include <glat/BSpline.h>

const std::vector<glm::vec3>& glat::BSpline::getControlPoints() {
	return m_ctrlPoints;
}

const std::vector<float>& glat::BSpline::getKnotValues() {
	return m_knotValues;
}

unsigned short glat::BSpline::getSplineDegree() {
	return m_degree;
}

void glat::BSpline::setKnotValues(std::initializer_list<float> knotValues) {
	setDirty(true);
	m_knotValues = knotValues;
}

void glat::BSpline::setControlPoints(std::initializer_list<glm::vec3> ctrlPoints) {
	setDirty(true);
	m_ctrlPoints = ctrlPoints;
}

glat::BSpline::BSpline(std::initializer_list<glm::vec3> ctrlPoints, std::initializer_list<float> knotValues) {
	setControlPoints(ctrlPoints);
	setKnotValues(knotValues);
	calculateSplineDegree();
}

glat::BSpline::BSpline(std::initializer_list<glm::vec3> ctrlPoints, unsigned short degree) {
	setControlPoints(ctrlPoints);
	m_degree = degree;
	calculateUniformKnotValues();
}

void glat::BSpline::calculateUniformKnotValues() {
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

void glat::BSpline::calculateSplineDegree() {
	m_degree = m_knotValues.size() - m_ctrlPoints.size() - 1;
	setDirty(true);
}

