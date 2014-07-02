#include <glat/Prismoid.h>
#include <glow/VertexAttributeBinding.h>
#include "ShaderSources.hpp"

static const char* geomShader = R"(
				#version 330
				### MATRIX_BLOCK ###

				layout(lines) in;
				layout(triangle_strip, max_vertices = 4) out;
			
				vec4 prismoid[8]; // Scratch space for the eight corners of the prismoid

				void emit(int a, int b, int c, int d) {
					gl_Position = prismoid[a]; EmitVertex();
					gl_Position = prismoid[b]; EmitVertex();
					gl_Position = prismoid[c]; EmitVertex();
					gl_Position = prismoid[d]; EmitVertex();
					EndPrimitive();
				}

				void main() {
					mat4 viewProjection = projectionMatrix * viewMatrix;
					// Compute orientation vectors for the two connecting faces:
					vec3 p0, p1, p2, p3;
					p0 = gl_in[0].gl_Position.xyz; p1 = gl_in[1].gl_Position.xyz;
					p2 = gl_in[2].gl_Position.xyz; p3 = gl_in[3].gl_Position.xyz;

					vec3 n0 = normalize(p1 - p0);
					vec3 n1 = normalize(p2 - p1);
					vec3 n2 = normalize(p3 - p2);
					vec3 u = normalize(n0 + n1);
					vec3 v = normalize(n1 + n2);

					// Declare scratch variables for basis vectors:
					vec3 i, j, k; float r = 0.05;

					// Compute face 1 of 2:
					// normals i need to be adjusted (TODO)
					j = u; i = normalize(vec3(1.0, 0.0, 1.0)); k = cross(i, j); i *= r; k *= r;
					prismoid[0] = viewProjection * vec4(p1 + i + k, 1.0);
					prismoid[1] = viewProjection * vec4(p1 + i - k, 1.0);
					prismoid[2] = viewProjection * vec4(p1 - i - k, 1.0);
					prismoid[3] = viewProjection * vec4(p1 - i + k, 1.0);

					// Compute face 2 of 2:
					j = v; i = vec3(1.0, 0.0, 0.0); k = cross(i, j); i *= r; k *= r;
					prismoid[4] = viewProjection * vec4(p2 + i + k, 1.0);
					prismoid[5] = viewProjection * vec4(p2 + i - k, 1.0);
					prismoid[6] = viewProjection * vec4(p2 - i - k, 1.0);
					prismoid[7] = viewProjection * vec4(p2 - i + k, 1.0);

					// Emit the six faces of the prismoid:
					emit(0, 1, 3, 2); emit(5, 4, 6, 7);
					emit(4, 5, 0, 1); emit(3, 2, 7, 6);
					emit(0, 3, 4, 7); emit(2, 1, 6, 5);
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

