#include <glannotations/BSpline.h>

const std::vector<glm::vec3>& glannotations::BSpline::getControlPoints() {
	return m_ctrlPoints;
}

const std::vector<float>& glannotations::BSpline::getKnotValues() {
	return m_knotValues;
}

size_t glannotations::BSpline::getSplineDegree() {
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

glannotations::BSpline::BSpline(std::initializer_list<glm::vec3> ctrlPoints, size_t degree) {
	setControlPoints(ctrlPoints);
	m_degree = degree;
	calculateUniformKnotValues();
}

void glannotations::BSpline::calculateUniformKnotValues() {
	m_knotValues.clear();

	// starting knot -- nonperiodic B-Spline
	m_knotValues.insert(m_knotValues.end(), m_degree, 0.f);

	size_t ctrlCount = m_ctrlPoints.size();
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

