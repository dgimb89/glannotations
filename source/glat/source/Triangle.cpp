#include <glat/Triangle.h>
#include <glat/Segment.h>

int glat::Triangle::intersect(const glat::Segment& S, glm::vec3& point) const {
	return S.intersect(*this, point);
}

glat::Triangle::Triangle(glm::vec3 A, glm::vec3 B, glm::vec3 C) {
	V0 = A;
	V1 = B;
	V2 = C;
}

glm::vec3 glat::Triangle::getNormal() const {
	return glm::cross(V1 - V0, V2 - V0);
}
