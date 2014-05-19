#pragma once

#include <glat/AbstractDrawingPrimitive.h>

namespace glat {
	class Quad : public glat::AbstractDrawingPrimitive {
	public:
		Quad();
		virtual void draw();
		virtual void setPosition(glm::vec3 ll, glm::vec3 lr, glm::vec3 ur, glm::mat4 modelViewProjection = glm::mat4());
	};
}