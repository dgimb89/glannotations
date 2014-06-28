#pragma once

#include <glat/AbstractDrawingPrimitive.h>

namespace glat {
	class Building : public glat::AbstractDrawingPrimitive {
	public:
		Building(gl::GLuint matricesBindingIndex = 0);
		virtual void draw();
		void setPosition(glm::vec3 llf, glm::vec3 urb);
	};
}