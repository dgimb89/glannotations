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
					box.widthSpan = vec4(widthSpan, 0.0);
					box.heightSpan = vec4(heightSpan, 0.0);
					box.depthSpan = vec4(depthSpan, 0.0);
					box.mvp = modelViewProjection;
					gl_Position = vec4(position, 1.0);
				}
				)";

static const char* geomShader = R"(
				#version 330

				layout(points) in;
				layout(triangle_strip, max_vertices = 18) out;

				in BoxData {
					vec4 widthSpan;
					vec4 heightSpan;
					vec4 depthSpan;
					mat4 mvp;
				} box[1];

				void main() {
					// front face

					gl_Position = box[0].mvp * (gl_in[0].gl_Position);
					EmitVertex();
					gl_Position = box[0].mvp * (gl_in[0].gl_Position + box[0].heightSpan);
					EmitVertex();
					gl_Position = box[0].mvp * (gl_in[0].gl_Position + box[0].widthSpan);
					EmitVertex();
					gl_Position = box[0].mvp * (gl_in[0].gl_Position + box[0].widthSpan + box[0].heightSpan);
					EmitVertex();

					// right face				
					gl_Position = box[0].mvp * (gl_in[0].gl_Position + box[0].widthSpan + box[0].depthSpan);
					EmitVertex();				
					gl_Position = box[0].mvp * (gl_in[0].gl_Position + box[0].widthSpan + box[0].depthSpan + box[0].heightSpan);
					EmitVertex();

					// back face
					gl_Position = box[0].mvp * (gl_in[0].gl_Position + box[0].depthSpan);
					EmitVertex();
					gl_Position = box[0].mvp * (gl_in[0].gl_Position + box[0].depthSpan + box[0].heightSpan);
					EmitVertex();

					// left face
					gl_Position = box[0].mvp * (gl_in[0].gl_Position);
					EmitVertex();
					gl_Position = box[0].mvp * (gl_in[0].gl_Position + box[0].heightSpan);
					EmitVertex();

					EndPrimitive();

					// top face
					gl_Position = box[0].mvp * (gl_in[0].gl_Position + box[0].heightSpan);
					EmitVertex();
					gl_Position = box[0].mvp * (gl_in[0].gl_Position + box[0].heightSpan + box[0].depthSpan);
					EmitVertex();
					gl_Position = box[0].mvp * (gl_in[0].gl_Position + box[0].heightSpan + box[0].widthSpan);
					EmitVertex();
					gl_Position = box[0].mvp * (gl_in[0].gl_Position + box[0].heightSpan + box[0].widthSpan + box[0].depthSpan);
					EmitVertex();

					EndPrimitive();

					// bottom face
					gl_Position = box[0].mvp * (gl_in[0].gl_Position + box[0].depthSpan);
					EmitVertex();
					gl_Position = box[0].mvp * (gl_in[0].gl_Position);
					EmitVertex();
					gl_Position = box[0].mvp * (gl_in[0].gl_Position + box[0].depthSpan + box[0].widthSpan);
					EmitVertex();
					gl_Position = box[0].mvp * (gl_in[0].gl_Position + box[0].widthSpan);
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

	m_widthSpan = new glow::Buffer(GL_ARRAY_BUFFER);
	m_heightSpan = new glow::Buffer(GL_ARRAY_BUFFER);
	m_depthSpan = new glow::Buffer(GL_ARRAY_BUFFER);

	m_vao->binding(0)->setAttribute(0);
	m_vao->binding(0)->setFormat(3, GL_FLOAT, GL_FALSE, 0);
	m_vao->enable(0);
}

void glat::Box::draw() {
	m_program->release();
	m_program->use();
	m_vao->drawArrays(GL_POINTS, 0, 1);
	m_program->release();
}

void glat::Box::setPosition(glm::vec3 llf, glm::vec3 widthSpan, glm::vec3 heightSpan, glm::vec3 depthSpan) {
	std::array<glm::vec3, 1> vertexArray{ { llf } };
	m_vao->binding(0)->setAttribute(0);
	m_vao->binding(0)->setFormat(3, GL_FLOAT, GL_FALSE, 0);
	m_vao->enable(0);
	m_positions->setData(vertexArray, GL_STATIC_DRAW);
	m_vao->binding(0)->setBuffer(m_positions, 0, sizeof(glm::vec3));

	std::array<glm::vec3, 1> widthSpanArray{ { widthSpan } };
	m_vao->binding(1)->setAttribute(1);
	m_vao->binding(1)->setFormat(3, GL_FLOAT, GL_FALSE, 0);
	m_vao->enable(1);
	m_widthSpan->setData(widthSpanArray, GL_STATIC_DRAW);
	m_vao->binding(1)->setBuffer(m_widthSpan, 0, sizeof(glm::vec3));

	std::array<glm::vec3, 1> heightSpanArray{ { heightSpan } };
	m_vao->binding(2)->setAttribute(2);
	m_vao->binding(2)->setFormat(3, GL_FLOAT, GL_FALSE, 0);
	m_vao->enable(2);
	m_heightSpan->setData(heightSpanArray, GL_STATIC_DRAW);
	m_vao->binding(2)->setBuffer(m_heightSpan, 0, sizeof(glm::vec3));

	std::array<glm::vec3, 1> depthSpanArray{ { depthSpan } };
	m_vao->binding(3)->setAttribute(3);
	m_vao->binding(3)->setFormat(3, GL_FLOAT, GL_FALSE, 0);
	m_vao->enable(3);
	m_depthSpan->setData(depthSpanArray, GL_STATIC_DRAW);
	m_vao->binding(3)->setBuffer(m_depthSpan, 0, sizeof(glm::vec3));
}