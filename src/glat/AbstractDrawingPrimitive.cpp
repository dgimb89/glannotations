#include <glat/AbstractDrawingPrimitive.h>

using namespace glat;

AbstractDrawingPrimitive::AbstractDrawingPrimitive(std::shared_ptr<glow::Texture> texture) {
	m_vao = new glow::VertexArrayObject();
	m_positions = new glow::Buffer(GL_ARRAY_BUFFER);
	m_texCoords = new glow::Buffer(GL_ARRAY_BUFFER);
	
	m_texture = texture;
}


void AbstractDrawingPrimitive::setColor(glm::vec4 color) {
	m_program->setUniform("color", color);
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
