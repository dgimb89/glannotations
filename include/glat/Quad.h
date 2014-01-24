#ifndef GLAT_QUAD_H
#define GLAT_QUAD_H

#include <glm/glm.hpp>

#include <glow/Shader.h>
#include <glow/Program.h>
#include <glow/Texture.h>
#include <glow/VertexArrayObject.h>
#include <glow/Buffer.h>

#include <glowutils/Camera.h>

namespace glat {
	class Quad {
	public:
		Quad(glow::Texture* distanceField);
		~Quad();
		void draw();

		void setPosition(glm::mat4 projection = glm::mat4());
		void setScale(glm::vec2 scale);
		void setOffset(glm::vec2 offset);
		void setOutline(float size, glm::vec3 color);
		void setBumpMap(float intensity);
		void setTextColor(glm::vec4 color);

	private:
		int m_style;

		glow::ref_ptr<glow::Shader> m_vertexShader;
		glow::ref_ptr<glow::Shader> m_fragmentShader;

		glow::ref_ptr<glow::Texture> m_texture;
		glow::ref_ptr<glow::Program> m_program;

		glow::ref_ptr<glow::VertexArrayObject> m_vao;
		glow::ref_ptr<glow::Buffer> m_buffer;
	};
}
#endif // !GLAT_QUAD_H
