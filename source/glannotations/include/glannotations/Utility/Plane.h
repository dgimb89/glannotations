#pragma once

#include <glm/glm.hpp>

#include <glannotations/glannotations_api.h>

namespace glat {
namespace Utility {
	struct GLANNOTATIONS_API Plane {
		Plane(glm::vec3 p, glm::vec3 n);

		glm::vec3 p, n;
	};
}
}