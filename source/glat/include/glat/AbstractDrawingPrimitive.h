#pragma once
#include <glm/glm.hpp>

#include <glat/DirtyFlagObject.h>
#include <glat/glat_api.h>

namespace glat {
	class GLAT_API AbstractDrawingPrimitive : public glo::Referenced {
	public:
		virtual void draw() = 0;
		virtual void setColor(glm::vec4 color) = 0;
		// important: bind matrices AFTER you've setup shader
		virtual void setBindingIndex(unsigned int bindingIndex) = 0;
	};
}
