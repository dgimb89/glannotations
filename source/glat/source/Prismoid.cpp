#include <glat/Prismoid.h>
#include <glow/VertexAttributeBinding.h>
#include "ShaderSources.hpp"

static const char* geomShader = R"(
				#version 330
				### MATRIX_BLOCK ###

				uniform vec3 annotationCenter;

				layout(lines) in;
				layout(triangle_strip, max_vertices = 6) out;

				void main() {
					mat4 viewProjection = projectionMatrix * viewMatrix;
					
					// calculate camera-dependant up vector
					vec3 up = viewMatrix[1];
					gl_Position = annotationCenter + up; EmitVertex();
					gl_Position = annotationCenter - up; EmitVertex();

					gl_Position = in[0] + up; EmitVertex();
					gl_Position = in[0] - up; EmitVertex();

					gl_Position = in[1] + up; EmitVertex();
					gl_Position = in[1] - up; EmitVertex();
					EndPrimitive();
				}
				)";


static const char* texturingFragShader = R"(
				#version 330
				uniform vec4 color;

				layout (location = 0) out vec4 fragColor;

				void main() {
					fragColor = color;
				}
				)";

void glat::Prismoid::draw() {
	m_program->release();
	m_program->use();
	m_vao->drawArrays(gl::GL_LINE_STRIP_ADJACENCY, 0, m_numVert);
	m_program->release();
}

void glat::Prismoid::setPosition(const std::vector<glm::vec3>& linestrip) {
	assert(linestrip.size() > 1);
	std::vector<glm::vec3> vertexArray;

	// we double input anchor and destination to be able to use GL_LINE_STRIP_ADJACENCY but keep it generalized to prismoids with generic chamfers
	vertexArray.push_back(linestrip.front() + glm::normalize(linestrip.front() - linestrip.at(1)));
	vertexArray.insert(vertexArray.begin() + 1, linestrip.begin(), linestrip.end());
	vertexArray.push_back(vertexArray.back() + glm::normalize(linestrip.back() - linestrip.at(linestrip.size() - 2)));

	m_numVert = vertexArray.size();
	m_positions->setData(vertexArray, gl::GL_STATIC_DRAW);
	m_vao->binding(0)->setBuffer(m_positions, 0, sizeof(glm::vec3));
}

glat::Prismoid::Prismoid(gl::GLuint matricesBindingIndex) {
	setupShader(glat::ShaderSources::passThroughVS, geomShader, texturingFragShader, matricesBindingIndex);

	// Position
	m_vao->binding(0)->setAttribute(0);
	m_vao->binding(0)->setFormat(3, gl::GL_FLOAT, gl::GL_FALSE, 0);
	m_vao->enable(0);
}

