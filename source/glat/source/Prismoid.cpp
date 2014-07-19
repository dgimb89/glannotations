#include <glat/Prismoid.h>
#include <glow/VertexAttributeBinding.h>
#include "ShaderSources.hpp"

static const char* geomShader = R"(
				#version 330
				### MATRIX_BLOCK ###

				uniform float labelStrength;
				uniform vec3 reference;

				layout(lines) in;
				layout(triangle_strip, max_vertices = 14) out;

				void main() {
					mat4 viewProjection = projectionMatrix * viewMatrix;
					// calculate camera-dependent up vector
					vec3 up = normalize(vec3(inverse(viewMatrix)[1])) * labelStrength;
				
					// calculate nearest distance point
					float invertedOrder = step(distance(gl_in[1].gl_Position.xyz, reference), distance(gl_in[0].gl_Position.xyz, reference));
					vec3 firstPoint = (1.0 - invertedOrder) * gl_in[0].gl_Position.xyz + invertedOrder * gl_in[1].gl_Position.xyz;
					vec3 secondPoint = (1.0 - invertedOrder) * gl_in[1].gl_Position.xyz + invertedOrder * gl_in[0].gl_Position.xyz;

					vec3 width = normalize(cross(up, firstPoint - reference)) * labelStrength;

					gl_Position = viewProjection * vec4(firstPoint + up - width, 1.0); EmitVertex();
					gl_Position = viewProjection * vec4(reference + up - width, 1.0); EmitVertex();
					gl_Position = viewProjection * vec4(firstPoint - up - width, 1.0); EmitVertex();
					gl_Position = viewProjection * vec4(reference - up - width, 1.0); EmitVertex();
					gl_Position = viewProjection * vec4(firstPoint - up + width, 1.0); EmitVertex();
					gl_Position = viewProjection * vec4(reference - up + width, 1.0); EmitVertex();
					gl_Position = viewProjection * vec4(firstPoint + up + width, 1.0); EmitVertex();
					gl_Position = viewProjection * vec4(reference + up + width, 1.0); EmitVertex();
					gl_Position = viewProjection * vec4(firstPoint + up - width, 1.0); EmitVertex();
					gl_Position = viewProjection * vec4(reference + up - width, 1.0); EmitVertex();
					EndPrimitive();

					gl_Position = viewProjection * vec4(firstPoint - up - width, 1.0); EmitVertex();
					gl_Position = viewProjection * vec4(firstPoint + up - width, 1.0); EmitVertex();
					gl_Position = viewProjection * vec4(secondPoint - up + width, 1.0); EmitVertex();
					gl_Position = viewProjection * vec4(secondPoint + up + width, 1.0); EmitVertex();
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
	m_vao->drawArrays(gl::GL_LINE_STRIP, 0, 2);
	m_program->release();
}

void glat::Prismoid::setPosition(const glm::vec3& a, const glm::vec3& b) {
	std::vector<glm::vec3> vertexArray{ a, b };
	m_positions->setData(vertexArray, gl::GL_STREAM_DRAW);
	m_vao->binding(0)->setBuffer(m_positions, 0, sizeof(glm::vec3));
}

glat::Prismoid::Prismoid() {
	setupShader(glat::ShaderSources::passThroughVS, geomShader, texturingFragShader);
	m_program->link();
	// Position
	m_vao->binding(0)->setAttribute(0);
	m_vao->binding(0)->setFormat(3, gl::GL_FLOAT, gl::GL_FALSE, 0);
	m_vao->enable(0);

	setLabelStrength(0.07f);
}

void glat::Prismoid::setReference(const glm::vec3& reference) {
	m_program->setUniform("reference", reference);
}

void glat::Prismoid::setLabelStrength(float strength) {
	m_program->setUniform("labelStrength", strength);
}

