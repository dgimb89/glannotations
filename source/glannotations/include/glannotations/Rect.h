#pragma once

#include <glannotations/DrawingPrimitiveBase.h>

namespace glannotations {
	class GLANNOTATIONS_API Rect : public glannotations::DrawingPrimitiveBase {
	public:
		Rect();
		virtual void draw();
		void setPosition(glm::vec3 ll, glm::vec3 lr, glm::vec3 ur);
	};
}