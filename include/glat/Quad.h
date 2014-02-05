#ifndef GLAT_QUAD_H
#define GLAT_QUAD_H

#include <glat/DirtyFlagObject.h>

#include <glm/glm.hpp>

#include <glow/Shader.h>
#include <glow/Program.h>
#include <glow/Texture.h>
#include <glow/VertexArrayObject.h>
#include <glow/Buffer.h>
#include <memory>

#include <glowutils/Camera.h>

namespace glat {
	class Quad : public glow::Referenced {
	public:
		Quad(std::shared_ptr<glow::Texture> distanceField);
		~Quad();
		void draw();

		void setPosition(glm::vec3 llf, glm::vec3 urb, glm::mat4 modelViewProjection = glm::mat4());
		void setScale(glm::vec2 scale);
		void setOffset(glm::vec2 offset);
		void setOutline(float size, glm::vec3 color);
		void setBumpMap(float intensity);
		void setTextColor(glm::vec4 color);

	private:
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
#endif // !GLAT_QUAD_H
