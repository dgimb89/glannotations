#include <glat/Quad.h>

#include <glow/Shader.h>
#include <glow/Program.h>

glat::Quad::Quad(glow::Texture* distanceField) : glowutils::ScreenAlignedQuad(distanceField) {
	m_vertexShaderSource = R"(
		#version 330

		layout (location = 0) in vec4 position;
		in vec2 scale;
		in vec2 offset;
		out vec2 v_uv;

		void main()
		{
			v_uv = vec2(position.x * 4f, position.y * 4.5f) * 0.5 + 0.5;
			gl_Position = position + vec4(0f, 0f, 0f, 0f);
		}
		)";

	m_fragmentShaderSource = R"(
		#version 330

		uniform sampler2D source;

		layout (location = 0) out vec4 fragColor;

		in vec2 v_uv;

		void main()
		{
			vec3 textColor = vec3(0f, 0f, 0f);
			float d = texture2D(source, v_uv).x - 0.48;	

			if (d > 0.0) {
				fragColor = vec4(textColor, 1f);
			} 
			else {
				fragColor = vec4(textColor, 0f);
			}
		}
		)";

	m_vertexShader = glow::Shader::fromString(GL_VERTEX_SHADER, m_vertexShaderSource);
	m_fragmentShader = glow::Shader::fromString(GL_FRAGMENT_SHADER, m_fragmentShaderSource);

	m_program = new glow::Program();
	m_program->attach(m_vertexShader, m_fragmentShader);
	//const GLfloat scale[2] = { 4.0, 4.5 };
	//m_program->setUniform("scale", scale);
}

void glat::Quad::draw() {
	glowutils::ScreenAlignedQuad::draw();
}