#ifndef GLAT_ABSTRACT_DRAWING_PRIMITIVE_H
#define GLAT_ABSTRACT_DRAWING_PRIMITIVE_H

#include <glat/DirtyFlagObject.h>

#include <glm/glm.hpp>

#include <glow/Shader.h>
#include <glow/Program.h>
#include <glow/Texture.h>
#include <glow/VertexArrayObject.h>
#include <glow/Buffer.h>
#include <memory>

namespace glat {
	class AbstractDrawingPrimitive : public glow::Referenced {
	public:
		AbstractDrawingPrimitive(std::shared_ptr<glow::Texture> distanceField);
		~AbstractDrawingPrimitive();

		virtual void draw() = 0;
		virtual void setPosition(glm::vec3 ll, glm::vec3 lr, glm::vec3 ur, glm::mat4 modelViewProjection = glm::mat4()) = 0;

		void setScale(glm::vec2 scale);
		void setOffset(glm::vec2 offset);
		void setOutline(float size, glm::vec3 color);
		void setBumpMap(float intensity);
		void setTextColor(glm::vec4 color);
		void setupShader(const char* fragShader, const char* vertShader);

	protected:
		void initialize();
		int m_style;

		glow::ref_ptr<glow::Shader> m_vertexShader;
		glow::ref_ptr<glow::Shader> m_fragmentShader;

		std::shared_ptr<glow::Texture> m_texture;
		glow::ref_ptr<glow::Program> m_program;

		glow::ref_ptr<glow::VertexArrayObject> m_vao;
		glow::ref_ptr<glow::Buffer> m_positions;
		glow::ref_ptr<glow::Buffer> m_texCoords;
	};
}
#endif // !GLAT_ABSTRACT_DRAWING_PRIMITIVE_H
