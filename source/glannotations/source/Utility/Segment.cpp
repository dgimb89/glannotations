#include <glannotations/Utility/Segment.h>
#include <glannotations/Utility/Triangle.h>
#include <glannotations/Utility/Plane.h>
#include <algorithm>

int glannotations::Utility::Segment::intersect(const glannotations::Utility::Triangle& T, glm::vec3& point) const {
	// http://geomalgorithms.com/a06-_intersect-2.html
	glm::vec3	u, v, n;              // triangle vectors
	glm::vec3	dir, w0, w;           // ray vectors
	float		r, a, b;              // params to calc ray-plane intersect

	// get triangle edge vectors and plane normal
	u = T.V1 - T.V0;
	v = T.V2 - T.V0;
	n = glm::cross(u, v);              // cross product
	if (n == (glm::vec3)0)             // triangle is degenerate
		return -1;                  // do not deal with this case

	dir = P1 - P0;              // ray direction vector
	w0 = P0 - T.V0;
	a = -glm::dot(n, w0);
	b = glm::dot(n, dir);
    if (std::abs(b) < std::numeric_limits<float>::epsilon()) {       // ray is  parallel to triangle plane
        if (std::abs(a) < std::numeric_limits<float>::epsilon()) // ray lies in triangle plane
			return -2;
		else return 0;              // ray disjoint from plane
	}

	// get intersect point of ray with triangle plane
	r = a / b;
	if (r < 0.0)                    // ray goes away from triangle
		return 0;                   // => no intersect
	// for a segment, also test if (r > 1.0) => no intersect
	if (r > 1.0)
		return 0;

	point = P0 + r * dir;            // intersect point of ray and plane

	// is I inside T?
	float    uu, uv, vv, wu, wv, D;
	uu = glm::dot(u, u);
	uv = glm::dot(u, v);
	vv = glm::dot(v, v);
	w = point - T.V0;
	wu = glm::dot(w, u);
	wv = glm::dot(w, v);
	D = uv * uv - uu * vv;

	// get and test parametric coords
	float s, t;
	s = (uv * wv - vv * wu) / D;
	if (s < 0.0 || s > 1.0)         // I is outside T
		return 0;
	t = (uv * wu - uu * wv) / D;
	if (t < 0.0 || (s + t) > 1.0)  // I is outside T
		return 0;

	return 1;                       // I is in T
}

glannotations::Utility::Segment::Segment(glm::vec3 A, glm::vec3 B) {
	P0 = A;
	P1 = B;
}

float glannotations::Utility::Segment::minDistanceToEndpoint(const glm::vec3& point) {
	return std::min(glm::distance(point, P0), glm::distance(point, P1));
}

int glannotations::Utility::Segment::orthographicProjection(const glm::vec3& point, glm::vec3& projectionPoint) const {
	glm::vec3 u(P1 - P0);
	float r = glm::dot((point - P0), u) / glm::dot(u, u);
	if (r < 0.f) {
		// before segment
		return 0;
	}
	else if (r > 1.f) {
		// after segment
		return 0;
	}

	projectionPoint = P0 + r*u;
	return 1;
}
