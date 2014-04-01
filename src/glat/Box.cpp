#include <glat/Box.h>
#include <glow/VertexAttributeBinding.h>
#include <array>

/// -------------------- Shader ---------------------
static const char*	vertShader = R"(
				#version 330
				uniform mat4 modelViewProjection;

				layout (location = 0) in vec3 position;
				layout (location = 1) in vec3 widthSpan;
				layout (location = 2) in vec3 heightSpan;
				layout (location = 3) in vec3 depthSpan;
				
				out BoxData {
					vec4 widthSpan;
					vec4 heightSpan;
					vec4 depthSpan;
					mat4 mvp;
				} box;

				void main()
				{
					box.mvp = modelViewProjection;
					gl_Position = vec4(position, 1.0);
				}
				)";

static const char* geomShader = R"(
				#version 330

				layout(lines) in;
				layout(triangle_strip, max_vertices = 14) out;

				in BoxData {
					mat4 mvp;
				} box[1];

				void main() {
					vec4 vertices[8];
					vertices[4] = vec4(gl_in[0].gl_Position.x, gl_in[0].gl_Position.y, gl_in[0].gl_Position.z, 1.0);
					vertices[0] = vec4(gl_in[0].gl_Position.x, gl_in[0].gl_Position.y, gl_in[1].gl_Position.z, 1.0);
					vertices[6] = vec4(gl_in[0].gl_Position.x, gl_in[1].gl_Position.y, gl_in[0].gl_Position.z, 1.0);
					vertices[2] = vec4(gl_in[0].gl_Position.x, gl_in[1].gl_Position.y, gl_in[1].gl_Position.z, 1.0);
					vertices[5] = vec4(gl_in[1].gl_Position.x, gl_in[0].gl_Position.y, gl_in[0].gl_Position.z, 1.0);
					vertices[1] = vec4(gl_in[1].gl_Position.x, gl_in[0].gl_Position.y, gl_in[1].gl_Position.z, 1.0);
					vertices[7] = vec4(gl_in[1].gl_Position.x, gl_in[1].gl_Position.y, gl_in[0].gl_Position.z, 1.0);
					vertices[3] = vec4(gl_in[1].gl_Position.x, gl_in[1].gl_Position.y, gl_in[1].gl_Position.z, 1.0);

					gl_Position = box[0].mvp * vertices[0];
					EmitVertex();
					gl_Position = box[0].mvp * vertices[1];
					EmitVertex();
					gl_Position = box[0].mvp * vertices[2];
					EmitVertex();
					gl_Position = box[0].mvp * vertices[3];
					EmitVertex();
					gl_Position = box[0].mvp * vertices[7];
					EmitVertex();
					gl_Position = box[0].mvp * vertices[1];
					EmitVertex();
					gl_Position = box[0].mvp * vertices[5];
					EmitVertex();
					gl_Position = box[0].mvp * vertices[4];
					EmitVertex();
					gl_Position = box[0].mvp * vertices[7];
					EmitVertex();
					gl_Position = box[0].mvp * vertices[6];
					EmitVertex();
					gl_Position = box[0].mvp * vertices[2];
					EmitVertex();
					gl_Position = box[0].mvp * vertices[4];
					EmitVertex();
					gl_Position = box[0].mvp * vertices[0];
					EmitVertex();
					gl_Position = box[0].mvp * vertices[1];
					EmitVertex();

					EndPrimitive();
				}
				)";

static const char* fragShader = R"(
				#version 330
				uniform vec4 color;

				layout (location = 0) out vec4 fragColor;

				void main()
				{
					fragColor = color;
				}
				)";


glat::Box::Box() : glat::AbstractDrawingPrimitive() {
	setupShader(fragShader, vertShader);
	m_geometryShader = glow::Shader::fromString(GL_GEOMETRY_SHADER, geomShader);
	m_program->attach(m_geometryShader);

	m_vao->binding(0)->setAttribute(0);
	m_vao->binding(0)->setFormat(3, GL_FLOAT, GL_FALSE, 0);
	m_vao->enable(0);
}

void glat::Box::draw() {
	m_program->release();
	m_program->use();
	m_vao->drawArrays(GL_LINES, 0, 2);
	m_program->release();
}

void glat::Box::setPosition(glm::vec3 llf, glm::vec3 urb) {
	std::array<glm::vec3, 2> vertexArray{ { llf, urb } };
	m_vao->binding(0)->setAttribute(0);
	m_vao->binding(0)->setFormat(3, GL_FLOAT, GL_FALSE, 0);
	m_vao->enable(0);
	m_positions->setData(vertexArray, GL_STATIC_DRAW);
	m_vao->binding(0)->setBuffer(m_positions, 0, sizeof(glm::vec3));
}