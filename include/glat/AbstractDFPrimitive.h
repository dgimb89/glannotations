#ifndef GLAT_ABSTRACT_DF_PRIMITIVE_H
#define GLAT_ABSTRACT_DF_PRIMITIVE_H

#include <glat/AbstractDrawingPrimitive.h>
#include <glow/Texture.h>
#include <memory>

namespace glat {
	class AbstractDFPrimitive : public glat::AbstractDrawingPrimitive {
	public:
		AbstractDFPrimitive(std::shared_ptr<glow::Texture> texture);

		virtual void setOutline(float size, glm::vec3 color);
		virtual void setPosition(glm::vec3 ll, glm::vec3 lr, glm::vec3 ur, glm::mat4 modelViewProjection = glm::mat4()) = 0;
		virtual void setBumpMap(float intensity);

	protected:
		int m_style;
		glow::ref_ptr<glow::Buffer> m_texCoords;
		std::shared_ptr<glow::Texture> m_texture;
	};
}

#endif // !GLAT_ABSTRACT_DF_PRIMITIVE_H