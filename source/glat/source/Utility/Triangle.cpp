#include <glat/Utility/Triangle.h>
#include <glat/Utility/Segment.h>

int glat::Utility::Triangle::intersect(const glat::Utility::Segment& S, glm::vec3& point) const {
	return S.intersect(*this, point);
}

glat::Utility::Triangle::Triangle(glm::vec3 A, glm::vec3 B, glm::vec3 C) {
	V0 = A;
	V1 = B;
	V2 = C;
}

glm::vec3 glat::Utility::Triangle::getNormal() const {
	return glm::cross(V1 - V0, V2 - V0);
}
