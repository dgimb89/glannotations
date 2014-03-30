#include <glat/AbstractDFPrimitive.h>

void glat::AbstractDFPrimitive::setOutline(float size, glm::vec3 color) {
	m_style |= 1;
	m_program->setUniform("outlineSize", size);
	m_program->setUniform("outlineColor", color);
	m_program->setUniform("style", m_style);
}

void glat::AbstractDFPrimitive::setBumpMap(float intensity) {
	m_style |= 2;
	m_program->setUniform("bumpIntensity", intensity);
	m_program->setUniform("style", m_style);
}

glat::AbstractDFPrimitive::AbstractDFPrimitive(std::shared_ptr<glow::Texture> texture) : glat::AbstractDrawingPrimitive(texture) {
	m_style = 0;
}
