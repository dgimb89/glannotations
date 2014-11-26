#pragma once

#include <glm/glm.hpp>

#include <glannotations/glannotations_api.h>

namespace glat {
namespace Utility {
	struct Segment;

	struct GLANNOTATIONS_API Triangle {
		Triangle(glm::vec3 A, glm::vec3 B, glm::vec3 C);
		int intersect(const glat::Utility::Segment& S, glm::vec3& point) const;
		glm::vec3 getNormal() const;

		glm::vec3 V0, V1, V2;
	};
}
}