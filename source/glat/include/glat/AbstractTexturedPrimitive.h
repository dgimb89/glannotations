#pragma once

#include <globjects/Texture.h>
#include <memory>

#include <glat/DrawingPrimitiveBase.h>

namespace glat {
	class AbstractTexturedPrimitive : public glat::DrawingPrimitiveBase {
	public:
		AbstractTexturedPrimitive(std::shared_ptr<glo::Texture> texture);
		virtual bool setPosition(glm::vec3 ll, glm::vec3 lr, glm::vec3 ur) = 0;
		virtual bool setViewportPosition(glm::vec2 ll, glm::vec2 lr, glm::vec2 ur) = 0;

		virtual void setOutline(float size, glm::vec3 color);
		virtual void setBumpMap(float intensity);

	protected:
		int m_style;
		glo::ref_ptr<glo::Buffer> m_texCoords;
		std::shared_ptr<glo::Texture> m_texture;
	};
}