#pragma once

#include <glat/DrawingPrimitiveBase.h>

namespace glat {
	class Box : public glat::DrawingPrimitiveBase {
	public:
		Box();
		virtual void draw();
		void setPosition(glm::vec3 llf, glm::vec3 urb);
	};
}