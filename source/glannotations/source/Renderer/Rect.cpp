#include <glannotations/Rect.h>

#include <globjects/VertexAttributeBinding.h>
#include <glbinding/gl/bitfield.h>
#include <glbinding/gl/functions.h>
#include <glbinding/gl/enum.h>
#include <array>
#include "ShaderSources.hpp"

/// -------------------- Shader ---------------------

static const char* geomShader = R"(
				#version 330
				### MATRIX_BLOCK ###

				layout(triangles) in;
				layout(triangle_strip, max_vertices = 4) out;		

				out vec3 v_normal;
				out vec3 v_vertex;

				void main() {
					mat4 viewProjection = projectionMatrix * viewMatrix;
					vec4 vertices[4];
					// front face
					vertices[0] = viewProjection * vec4(gl_in[0].gl_Position.xyz, 1.0);
					vertices[1] = viewProjection * vec4(gl_in[1].gl_Position.xyz, 1.0);
					vertices[2] = viewProjection * vec4(gl_in[2].gl_Position.xyz, 1.0);
					vertices[3] = viewProjection * vec4(gl_in[0].gl_Position.xyz + (gl_in[2].gl_Position.xyz - gl_in[1].gl_Position.xyz), 1.0);

					// lateral surface
					gl_Position = vertices[3];
					EmitVertex();
					gl_Position = vertices[0];
					EmitVertex();
					gl_Position = vertices[2];
					EmitVertex();
					gl_Position = vertices[1];
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


glannotations::Rect::Rect() {
	setupShader(glannotations::ShaderSources::passThroughVS, geomShader, texturingFragShader);
	m_vao->binding(0)->setAttribute(0);
	m_vao->binding(0)->setFormat(3, gl::GL_FLOAT, gl::GL_FALSE, 0);
	m_vao->enable(0);
}

void glannotations::Rect::draw() {
	m_program->use();
	m_vao->drawArrays(gl::GL_TRIANGLES, 0, 3);
	m_program->release();
}

void glannotations::Rect::setPosition(glm::vec3 ll, glm::vec3 lr, glm::vec3 ur) {
	std::array<glm::vec3, 3> vertexArray{ { ll, lr, ur } };
	m_vao->binding(0)->setAttribute(0);
	m_vao->binding(0)->setFormat(3, gl::GL_FLOAT, gl::GL_FALSE, 0);
	m_vao->enable(0);
	m_positions->setData(vertexArray, gl::GL_STATIC_DRAW);
	m_vao->binding(0)->setBuffer(m_positions, 0, sizeof(glm::vec3));
}