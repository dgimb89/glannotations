#pragma once

#include <glat/AbstractDrawingPrimitive.h>
#include <vector>

namespace glat {
	class Prismoid : public glat::AbstractDrawingPrimitive {
	public:
		Prismoid();
		void setLabelStrength(float strength);
		virtual void draw();
		void setPosition(const glm::vec3& a, const glm::vec3& b);
		void setReference(const glm::vec3& reference);
	};
}