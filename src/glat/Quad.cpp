#include <glat/Quad.h>
#include "ShaderSources.hpp"

#include <glow/Shader.h>
#include <glow/Program.h>

using namespace glat;

Quad::Quad(glow::Texture* distanceField) : glowutils::ScreenAlignedQuad(distanceField) {

	m_vertexShader = glow::Shader::fromString(GL_VERTEX_SHADER, ShaderSource::vertexShaderSource);
	m_fragmentShader = glow::Shader::fromString(GL_FRAGMENT_SHADER, ShaderSource::fragmentShaderSource);

	m_program->release();
	m_program = new glow::Program();
	m_program->attach(m_vertexShader, m_fragmentShader);

	m_program->setUniform("scale", glm::vec2(1.0f, 1.0f));
	m_program->setUniform("offset", glm::vec2(0.0f, 0.0f));
}


void Quad::draw() {
	m_program->release();
	glowutils::ScreenAlignedQuad::draw();
}


void Quad::setScale(glm::vec2 scale) {
	m_program->setUniform("scale", scale);
}


void Quad::setOffset(glm::vec2 offset) {
	m_program->setUniform("offset", offset);
}

void Quad::setOutline(float size, glm::vec3 color) {
	m_program->setUniform("outlineSize", size);
	m_program->setUniform("outlineColor", color);
	m_program->setUniform("style", 1);
}