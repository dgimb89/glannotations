#pragma once

#include <glow/Texture.h>
#include <memory>

#include <glat/AbstractDrawingPrimitive.h>

namespace glat {
	class AbstractTexturedPrimitive : public glat::AbstractDrawingPrimitive {
	public:
		AbstractTexturedPrimitive(std::shared_ptr<glow::Texture> texture);
		virtual void setPosition(glm::vec3 ll, glm::vec3 lr, glm::vec3 ur, glm::mat4 modelViewProjection = glm::mat4()) = 0;

		virtual void setOutline(float size, glm::vec3 color);
		virtual void setBumpMap(float intensity);

	protected:
		int m_style;
		glow::ref_ptr<glow::Buffer> m_texCoords;
		std::shared_ptr<glow::Texture> m_texture;
	};
}