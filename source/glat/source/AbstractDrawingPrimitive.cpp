#include <glat/AbstractDrawingPrimitive.h>

using namespace glat;

AbstractDrawingPrimitive::AbstractDrawingPrimitive() {
	m_vao = new glow::VertexArrayObject();
	m_positions = new glow::Buffer();
}


void AbstractDrawingPrimitive::setColor(glm::vec4 color) {
	m_program->setUniform("color", color);
}


AbstractDrawingPrimitive::~AbstractDrawingPrimitive() {
	m_program->release();
}

void glat::AbstractDrawingPrimitive::setupShader(const char* fragShader, const char* vertShader) {
	m_vertexShader = glow::Shader::fromString(gl::GL_VERTEX_SHADER, vertShader);
	m_fragmentShader = glow::Shader::fromString(gl::GL_FRAGMENT_SHADER, fragShader);
	
	m_program = new glow::Program();
	m_program->attach(m_vertexShader, m_fragmentShader);
}

void glat::AbstractDrawingPrimitive::setModelViewProjection(glm::mat4 modelViewProjection /*= glm::mat4()*/) {
	m_program->setUniform("modelViewProjection", modelViewProjection);
}
