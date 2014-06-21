#pragma once

#include <glm/glm.hpp>

#include <glat/glat_api.h>

namespace glat {
namespace Utility {
	struct GLAT_API Plane {
		Plane(glm::vec3 p, glm::vec3 n);

		glm::vec3 p, n;
	};
}
}