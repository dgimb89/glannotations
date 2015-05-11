#pragma once

#include <glannotations/glannotations_api.h>

#include <glm/glm.hpp>

template struct GLANNOTATIONS_API glm::detail::tvec3 < float, glm::precision::defaultp > ;

namespace glannotations {
	class GLANNOTATIONS_API BoundingBox {
	public:
		BoundingBox();
		BoundingBox(glm::vec3 llf, glm::vec3 urb);
		void extendBy(glm::vec3 vertex);
		const glm::vec3& getLLF() const;
		const glm::vec3& getURB() const;

	private:
		glm::vec3 m_llf, m_urb;
	};
}
