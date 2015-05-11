#pragma once

#include <glm/glm.hpp>

#include <glannotations/glannotations_api.h>

template struct GLANNOTATIONS_API glm::detail::tvec3 < float , glm::precision::defaultp> ;

namespace glannotations {
namespace Utility {
	struct GLANNOTATIONS_API Plane {
		Plane(glm::vec3 p, glm::vec3 n);

		glm::vec3 p, n;
	};
}
}