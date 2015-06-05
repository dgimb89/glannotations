#pragma once

#include <globjects/Texture.h>
#include <memory>

#include <glannotations/DrawingPrimitiveBase.h>

namespace glannotations {
	class AbstractTexturedPrimitive : public glannotations::DrawingPrimitiveBase {
	public:
		AbstractTexturedPrimitive(std::shared_ptr<globjects::Texture> texture);
		virtual glm::vec2 getExtends() const = 0; ////todo:anne maybe remove to subclass, then cast
		virtual bool setPosition(glm::vec3 ll, glm::vec3 lr, glm::vec3 ur) = 0; //todo:anne maybe remove to subclass, then cast
		virtual bool setViewportPosition(glm::vec2 ll, glm::vec2 lr, glm::vec2 ur) = 0; //todo:anne maybe remove to subclass, then cast

		virtual void setOutline(float size, glm::vec3 color);
		virtual void setBumpMap(float intensity);

	protected:
		int m_style;
		globjects::ref_ptr<globjects::Buffer> m_texCoords;
		std::shared_ptr<globjects::Texture> m_texture;
	};
}