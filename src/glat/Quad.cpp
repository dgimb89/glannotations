#include <glat/Quad.h>
#include "ShaderSources.hpp"

#include <glow/Array.h>
#include <glow/VertexAttributeBinding.h>

#include <glm/gtc/matrix_transform.hpp>

using namespace glat;

Quad::Quad(std::shared_ptr<glow::Texture> distanceField) {
	m_style = 0;
	m_vertexShader = glow::Shader::fromString(GL_VERTEX_SHADER, ShaderSource::vertexShaderSource);
	m_fragmentShader = glow::Shader::fromString(GL_FRAGMENT_SHADER, ShaderSource::fragmentShaderSource);

	m_texture = distanceField;
	m_program = new glow::Program();
	m_program->attach(m_vertexShader, m_fragmentShader);

	initialize();
}


void Quad::initialize() {
	m_vao = new glow::VertexArrayObject();
	m_positions = new glow::Buffer(GL_ARRAY_BUFFER);
	m_texCoords = new glow::Buffer(GL_ARRAY_BUFFER);

	// Position
	m_vao->binding(0)->setAttribute(0);
	m_vao->binding(0)->setFormat(4, GL_FLOAT, GL_FALSE, 0);
	m_vao->enable(0);

	// Texture Coordinates
	glow::Array<glm::vec2> textureArray = glow::Vec2Array()
		<< glm::vec2(1.f, 0.f)
		<< glm::vec2(1.f, 1.f)
		<< glm::vec2(0.f, 0.f)
		<< glm::vec2(0.f, 1.f);

	m_texCoords->setData(textureArray, GL_STATIC_DRAW);

	m_vao->binding(1)->setAttribute(1);
	m_vao->binding(1)->setBuffer(m_texCoords, 0, sizeof(glm::vec2));
	m_vao->binding(1)->setFormat(2, GL_FLOAT, GL_FALSE, 0);
	m_vao->enable(1);

	setPosition(glm::vec3(-1.f, -1.f, 0.f), glm::vec3(1.f, 1.f, 0.f));

	m_program->setUniform("source", 0);
}


void Quad::setPosition(glm::vec3 llf, glm::vec3 urb, glm::mat4 modelViewProjection /*=glm::mat4()*/) {

	glow::Array<glm::vec4> vertexArray = glow::Vec4Array()
		<< glm::vec4( urb.x, llf.y, llf.z, 1.f)
		<< glm::vec4( urb.x, urb.y, llf.z, 1.f)
		<< glm::vec4( llf.x, llf.y, urb.z, 1.f)
		<< glm::vec4( llf.x, urb.y, urb.z, 1.f);

	m_positions->setData(vertexArray, GL_STATIC_DRAW);
	m_vao->binding(0)->setBuffer(m_positions, 0, sizeof(glm::vec4));

	m_program->setUniform("modelViewProjection", modelViewProjection);
}


void Quad::draw() {
	m_program->release();
	if (m_texture)
	{
		glActiveTexture(GL_TEXTURE0);
		CheckGLError();

		m_texture->bind();
	}

	m_program->use();
	m_vao->drawArrays(GL_TRIANGLE_STRIP, 0, 4);
	m_program->release();

	if (m_texture)
		m_texture->unbind();
}


void Quad::setScale(glm::vec2 scale) {
	m_program->setUniform("scale", scale);
}


void Quad::setOffset(glm::vec2 offset) {
	m_program->setUniform("offset", offset);
}


void Quad::setOutline(float size, glm::vec3 color) {
	m_style |= 1;
	m_program->setUniform("outlineSize", size);
	m_program->setUniform("outlineColor", color);
	m_program->setUniform("style", m_style);
}


void Quad::setBumpMap(float intensity) {
	m_style |= 2;
	m_program->setUniform("bumpIntensity", intensity);
	m_program->setUniform("style", m_style);
}


void Quad::setTextColor(glm::vec4 color) {
	m_program->setUniform("textColor", color);
}


Quad::~Quad() {
	m_program->release();
}