#pragma once

#include <glm/glm.hpp>

#include <glannotations/glannotations_api.h>

namespace glannotations {
namespace Utility {
	struct Triangle;
	struct Plane;

	struct GLANNOTATIONS_API Segment {
		Segment(glm::vec3 A, glm::vec3 B);
		/************************************************************************/
		/* return:
		-2 = are in same plane
		-1 = triangle is degenerated
		0 = disjoint (no intersection)
		1 = intersect (saved in input "point")
		/************************************************************************/
		int intersect(const glannotations::Utility::Triangle& T, glm::vec3& point) const;
		int orthographicProjection(const glm::vec3& point, glm::vec3& projectionPoint) const;
		float minDistanceToEndpoint(const glm::vec3& point);
		glm::vec3 P0, P1;
	};
}
}