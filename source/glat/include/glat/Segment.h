#pragma once

#include <glm/glm.hpp>

#include <glat/glat_api.h>

namespace glat {
	struct Triangle;

	struct GLAT_API Segment {
		Segment(glm::vec3 A, glm::vec3 B);
		/************************************************************************/
		/* return:	
		-2 = are in same plane
		-1 = triangle is degenerated
		0 = disjoint (no intersection)
		1 = intersect (saved in input "point")
		/************************************************************************/
		int intersect(const glat::Triangle& T, glm::vec3& point) const;

		glm::vec3 P0, P1;
	};
}