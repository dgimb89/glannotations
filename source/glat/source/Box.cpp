#include <glat/Box.h>

#include <globjects/VertexAttributeBinding.h>
#include <glbinding/gl/bitfield.h>
#include <array>
#include "ShaderSources.hpp"

/// -------------------- Shader ---------------------

static const char* geomShader = R"(
				#version 330
				### MATRIX_BLOCK ###

				layout(lines) in;
				layout(triangle_strip, max_vertices = 18) out;		

				out vec3 v_normal;
				out vec3 v_vertex;

				void main() {
					mat4 viewProjection = projectionMatrix * viewMatrix;
					vec4 vertices[8];
					// front face
					vertices[0] = viewProjection * vec4(gl_in[0].gl_Position.x, gl_in[0].gl_Position.y, gl_in[0].gl_Position.z, 1.0);
					vertices[1] = viewProjection * vec4(gl_in[0].gl_Position.x, gl_in[1].gl_Position.y, gl_in[0].gl_Position.z, 1.0);
					vertices[2] = viewProjection * vec4(gl_in[1].gl_Position.x, gl_in[0].gl_Position.y, gl_in[0].gl_Position.z, 1.0);
					vertices[3] = viewProjection * vec4(gl_in[1].gl_Position.x, gl_in[1].gl_Position.y, gl_in[0].gl_Position.z, 1.0);

					// back face
					vertices[4] = viewProjection * vec4(gl_in[1].gl_Position.x, gl_in[0].gl_Position.y, gl_in[1].gl_Position.z, 1.0);
					vertices[5] = viewProjection * vec4(gl_in[1].gl_Position.x, gl_in[1].gl_Position.y, gl_in[1].gl_Position.z, 1.0);
					vertices[6] = viewProjection * vec4(gl_in[0].gl_Position.x, gl_in[0].gl_Position.y, gl_in[1].gl_Position.z, 1.0);
					vertices[7] = viewProjection * vec4(gl_in[0].gl_Position.x, gl_in[1].gl_Position.y, gl_in[1].gl_Position.z, 1.0);

					// lateral surface
					gl_Position = vertices[0];
					EmitVertex();
					gl_Position = vertices[1];
					EmitVertex();
					gl_Position = vertices[2];
					EmitVertex();
					gl_Position = vertices[3];
					EmitVertex();
					gl_Position = vertices[4];
					EmitVertex();
					gl_Position = vertices[5];
					EmitVertex();
					gl_Position = vertices[6];
					EmitVertex();
					gl_Position = vertices[7];
					EmitVertex();
					gl_Position = vertices[0];
					EmitVertex();
					gl_Position = vertices[1];
					EmitVertex();
					EndPrimitive();

					// top surface
					gl_Position = vertices[1];
					EmitVertex();
					gl_Position = vertices[7];
					EmitVertex();
					gl_Position = vertices[3];
					EmitVertex();
					gl_Position = vertices[5];
					EmitVertex();
					EndPrimitive();

					// bottom surface
					gl_Position = vertices[6];
					EmitVertex();
					gl_Position = vertices[0];
					EmitVertex();
					gl_Position = vertices[4];
					EmitVertex();
					gl_Position = vertices[2];
				
					EmitVertex();
					EndPrimitive();
					
				}
				)";

static const char* texturingFragShader = R"(
				#version 330
				uniform vec4 color;

				layout (location = 0) out vec4 fragColor;

				void main()
				{
					fragColor = color;
				}
				)";


glat::Box::Box() {
	setupShader(glat::ShaderSources::passThroughVS, geomShader, texturingFragShader);
	m_vao->binding(0)->setAttribute(0);
	m_vao->binding(0)->setFormat(3, gl::GL_FLOAT, gl::GL_FALSE, 0);
	m_vao->enable(0);
}

void glat::Box::draw() {
	m_program->release();
	gl::glPushAttrib(gl::GL_POLYGON_BIT);
	gl::glDisable(gl::GL_CULL_FACE);

	m_program->use();
	m_vao->drawArrays(gl::GL_LINES, 0, 2);
	m_program->release();

	gl::glPopAttrib();
}

void glat::Box::setPosition(glm::vec3 llf, glm::vec3 urb) {
	std::array<glm::vec3, 2> vertexArray{ { llf, urb } };
	m_vao->binding(0)->setAttribute(0);
	m_vao->binding(0)->setFormat(3, gl::GL_FLOAT, gl::GL_FALSE, 0);
	m_vao->enable(0);
	m_positions->setData(vertexArray, gl::GL_STATIC_DRAW);
	m_vao->binding(0)->setBuffer(m_positions, 0, sizeof(glm::vec3));
}