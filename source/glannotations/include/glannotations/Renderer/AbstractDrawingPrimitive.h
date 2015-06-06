#pragma once
#include <glm/glm.hpp>

#include <glannotations/Common/DirtyFlagObject.h>
#include <glannotations/glannotations_api.h>

namespace glannotations {
	class GLANNOTATIONS_API AbstractDrawingPrimitive : public globjects::Referenced {
	public:
		virtual void draw() = 0;
		virtual void setColor(glm::vec4 color) = 0;
		// important: bind matrices AFTER you've setup shader
		virtual void setBindingIndex(unsigned int bindingIndex) = 0;
	};
}
