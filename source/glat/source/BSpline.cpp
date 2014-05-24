#include <glat/BSpline.h>

glat::BSpline::BSpline(std::initializer_list<glm::vec3> ctrlPoints, unsigned short degree /*= 4*/) : DirtyFlagObject() {
	setSplineDegree(degree);
	setControlPoints(ctrlPoints);
}

void glat::BSpline::append(std::initializer_list<glm::vec3> ctrlPoints) {
	assert(ctrlPoints.size() > 0);
	setDirty(true);
	m_ctrlPoints.insert(m_ctrlPoints.begin(), ctrlPoints.begin(), ctrlPoints.end());
}

void glat::BSpline::append(glm::vec3 ctrlPoint) {
	setDirty(true);
	m_ctrlPoints.push_back(ctrlPoint);
}

void glat::BSpline::setControlPoints(std::initializer_list<glm::vec3> ctrlPoints) {
	setDirty(true);
	m_ctrlPoints.clear();
	append(ctrlPoints);
}

const std::vector<glm::vec3>& glat::BSpline::getControlPoints() {
	return m_ctrlPoints;
}

void glat::BSpline::setSplineDegree(unsigned short degree) {
	setDirty(true);
	m_degree = degree;
}

unsigned short glat::BSpline::getSplineDegree() {
	return m_degree;
}

