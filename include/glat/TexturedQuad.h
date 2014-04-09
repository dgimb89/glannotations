#pragma once

#include <glat/AbstractDFPrimitive.h>

namespace glat {
	class TexturedQuad : public glat::AbstractDFPrimitive {
	public:
		TexturedQuad(std::shared_ptr<glow::Texture> texture, bool isDistanceField);
		
		virtual void draw();
		virtual void setPosition(glm::vec3 ll, glm::vec3 lr, glm::vec3 ur, glm::mat4 modelViewProjection = glm::mat4());
	};
}
