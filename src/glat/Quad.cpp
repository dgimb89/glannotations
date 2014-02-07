#include <glat/Quad.h>
#include <glow/VertexAttributeBinding.h>
#include "ShaderSources.hpp"

glat::Quad::Quad(std::shared_ptr<glow::Texture> distanceField) : glat::AbstractDrawingPrimitive(distanceField) {
	setupShader(ShaderSource::fragmentShaderSource, ShaderSource::vertexShaderSource);
	// Position
	m_vao->binding(0)->setAttribute(0);
	m_vao->binding(0)->setFormat(4, GL_FLOAT, GL_FALSE, 0);
	m_vao->enable(0);

	// Texture Coordinates
	std::array<glm::vec2, 4> textureArray{ {
		glm::vec2(1.f, 0.f)
		, glm::vec2(1.f, 1.f)
		, glm::vec2(0.f, 0.f)
		, glm::vec2(0.f, 1.f)
	} };

	m_texCoords->setData(textureArray, GL_STATIC_DRAW);

	m_vao->binding(1)->setAttribute(1);
	m_vao->binding(1)->setBuffer(m_texCoords, 0, sizeof(glm::vec2));
	m_vao->binding(1)->setFormat(4, GL_FLOAT, GL_FALSE, 0);
	m_vao->enable(1);

	setPosition(glm::vec3(-1.f, -1.f, 0.f), glm::vec3(1.f, -1.f, 0.f), glm::vec3(1.f, 1.f, 0.f));

	m_program->setUniform("source", 0);
}


void glat::Quad::setPosition(glm::vec3 ll, glm::vec3 lr, glm::vec3 ur, glm::mat4 modelViewProjection /*= glm::mat4()*/) {

	std::array<glm::vec3, 4> vertexArray{ {
			glm::vec3(lr.x, lr.y, lr.z)
		,	glm::vec3(ur.x, ur.y, ur.z)
		,	glm::vec3(ll.x, ll.y, ll.z)
		,	glm::vec3(ll.x + (ur.x - lr.x), ll.y + (ur.y - lr.y), ll.z + (ur.z - lr.z))
	}};


	m_positions->setData(vertexArray, GL_STATIC_DRAW);
	m_vao->binding(0)->setBuffer(m_positions, 0, sizeof(glm::vec3));

	m_program->setUniform("modelViewProjection", modelViewProjection);
}


void glat::Quad::draw() {
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