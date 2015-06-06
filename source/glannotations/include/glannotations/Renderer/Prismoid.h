#pragma once

#include <glannotations/Renderer/DrawingPrimitiveBase.h>
#include <vector>

namespace glannotations {
	class Prismoid : public glannotations::DrawingPrimitiveBase {
	public:
		Prismoid();
		void setLabelStrength(float strength);
		virtual void draw();
		void setPosition(const glm::vec3& a, const glm::vec3& b);
		void setReference(const glm::vec3& reference);
	};
}