#include <glat/AbstractDrawingPrimitive.h>

using namespace glat;

AbstractDrawingPrimitive::AbstractDrawingPrimitive(std::shared_ptr<glow::Texture> distanceField) {
	m_vao = new glow::VertexArrayObject();
	m_positions = new glow::Buffer(GL_ARRAY_BUFFER);
	m_texCoords = new glow::Buffer(GL_ARRAY_BUFFER);
	
	m_style = 0;
	m_texture = distanceField;
}


void AbstractDrawingPrimitive::setScale(glm::vec2 scale) {
	m_program->setUniform("scale", scale);
}


void AbstractDrawingPrimitive::setOffset(glm::vec2 offset) {
	m_program->setUniform("offset", offset);
}


void AbstractDrawingPrimitive::setOutline(float size, glm::vec3 color) {
	m_style |= 1;
	m_program->setUniform("outlineSize", size);
	m_program->setUniform("outlineColor", color);
	m_program->setUniform("style", m_style);
}


void AbstractDrawingPrimitive::setBumpMap(float intensity) {
	m_style |= 2;
	m_program->setUniform("bumpIntensity", intensity);
	m_program->setUniform("style", m_style);
}


void AbstractDrawingPrimitive::setTextColor(glm::vec4 color) {
	m_program->setUniform("textColor", color);
}


AbstractDrawingPrimitive::~AbstractDrawingPrimitive() {
	m_program->release();
}

void glat::AbstractDrawingPrimitive::setupShader(const char* fragShader, const char* vertShader) {
	m_vertexShader = glow::Shader::fromString(GL_VERTEX_SHADER, vertShader);
	m_fragmentShader = glow::Shader::fromString(GL_FRAGMENT_SHADER, fragShader);
	
	m_program = new glow::Program();
	m_program->attach(m_vertexShader, m_fragmentShader);
}
