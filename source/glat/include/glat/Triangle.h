#pragma once

#include <glm/glm.hpp>

#include <glat/glat_api.h>

namespace glat {
	struct Segment;

	struct GLAT_API Triangle {
		Triangle(glm::vec3 A, glm::vec3 B, glm::vec3 C);
		int intersect(const glat::Segment& S, glm::vec3& point) const;
		glm::vec3 getNormal() const;

		glm::vec3 V0, V1, V2;
	};
}