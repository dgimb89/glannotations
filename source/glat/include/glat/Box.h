#pragma once

#include <glat/AbstractDrawingPrimitive.h>

namespace glat {
	class Box : public glat::AbstractDrawingPrimitive {
	public:
		Box(gl::GLuint matricesBindingIndex = 0);
		virtual void draw();
		void setPosition(glm::vec3 llf, glm::vec3 urb);
	};
}