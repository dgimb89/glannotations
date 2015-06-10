#pragma once

#include <glannotations/Renderer/DrawingPrimitiveBase.h>

namespace glannotations {
	class GLANNOTATIONS_API Box : public glannotations::DrawingPrimitiveBase {
	public:
		Box();
		virtual void draw();
		void setPosition(glm::vec3 llf, glm::vec3 urb);
	};
}