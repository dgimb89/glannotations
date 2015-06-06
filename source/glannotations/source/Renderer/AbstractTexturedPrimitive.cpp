#include <glannotations/AbstractTexturedPrimitive.h>

glannotations::AbstractTexturedPrimitive::AbstractTexturedPrimitive(std::shared_ptr<globjects::Texture> texture) {
	m_style = 0;
	m_texCoords = new globjects::Buffer();
	m_texture = texture;
}

void glannotations::AbstractTexturedPrimitive::setOutline(float size, glm::vec3 color) {
	m_style |= 1;
	m_program->setUniform("outlineSize", size);
	m_program->setUniform("outlineColor", color);
	m_program->setUniform("style", m_style);
}

void glannotations::AbstractTexturedPrimitive::setBumpMap(float intensity) {
	m_style |= 2;
	m_program->setUniform("bumpIntensity", intensity);
	m_program->setUniform("style", m_style);
}
