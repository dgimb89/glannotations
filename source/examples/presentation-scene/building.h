#pragma once

#include <glat/DrawingPrimitiveBase.h>

namespace glat {
	class Building : public glat::DrawingPrimitiveBase {
	public:
		Building(unsigned int matricesBindingIndex = 0);
		virtual void draw() override;
		void setPosition(glm::vec3 llf, glm::vec3 urb);
	};
}