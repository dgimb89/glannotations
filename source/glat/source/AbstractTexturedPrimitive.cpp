#include <glat/AbstractTexturedPrimitive.h>

glat::AbstractTexturedPrimitive::AbstractTexturedPrimitive(std::shared_ptr<glow::Texture> texture) : glat::AbstractDrawingPrimitive() {
	m_style = 0;
	m_texCoords = new glow::Buffer();
	m_texture = texture;
}

void glat::AbstractTexturedPrimitive::setOutline(float size, glm::vec3 color) {
	m_style |= 1;
	m_program->setUniform("outlineSize", size);
	m_program->setUniform("outlineColor", color);
	m_program->setUniform("style", m_style);
}

void glat::AbstractTexturedPrimitive::setBumpMap(float intensity) {
	m_style |= 2;
	m_program->setUniform("bumpIntensity", intensity);
	m_program->setUniform("style", m_style);
}
