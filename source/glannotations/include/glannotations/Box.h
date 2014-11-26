#pragma once

#include <glannotations/DrawingPrimitiveBase.h>

namespace glat {
	class GLANNOTATIONS_API Box : public glat::DrawingPrimitiveBase {
	public:
		Box();
		virtual void draw();
		void setPosition(glm::vec3 llf, glm::vec3 urb);
	};
}