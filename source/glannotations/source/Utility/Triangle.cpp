#include <glannotations/Utility/Triangle.h>
#include <glannotations/Utility/Segment.h>

int glannotations::Utility::Triangle::intersect(const glannotations::Utility::Segment& S, glm::vec3& point) const {
	return S.intersect(*this, point);
}

glannotations::Utility::Triangle::Triangle(glm::vec3 A, glm::vec3 B, glm::vec3 C) {
	V0 = A;
	V1 = B;
	V2 = C;
}

glm::vec3 glannotations::Utility::Triangle::getNormal() const {
	return glm::cross(V1 - V0, V2 - V0);
}
