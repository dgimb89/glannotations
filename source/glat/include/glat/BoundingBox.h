#pragma once
#define NOMINMAX

#include <glat/glat_api.h>

#include <glm/glm.hpp>

namespace glat {
	class GLAT_API BoundingBox {
	public:
		BoundingBox();
		void extendBy(glm::vec3 vertex);
		const glm::vec3& getLLF() const;
		const glm::vec3& getURB() const;

	protected:
		glm::vec3 m_llf, m_urb;
	};
}