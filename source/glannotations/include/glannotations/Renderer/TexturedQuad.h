#pragma once

#include <glannotations/Renderer/AbstractTexturedPrimitive.h>

namespace glannotations {
	class TexturedQuad : public glannotations::AbstractTexturedPrimitive {
	public:
		TexturedQuad(std::shared_ptr<globjects::Texture> texture, bool isDistanceField);
		
		virtual void draw();
		virtual void setPosition(glm::vec3 ll, glm::vec3 lr, glm::vec3 ur, glm::mat4 modelViewProjection = glm::mat4());
		
	protected:
	};
}
