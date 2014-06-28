#include <glowutils/StringTemplate.h>
#include <glowbase/StaticStringSource.h>
#include "ShaderSources.hpp"

#include <glat/AbstractDrawingPrimitive.h>
#include <glat/AbstractAnnotation.h>

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

void glat::AbstractDrawingPrimitive::setupShader(const char* vertShader, const char* fragShader, gl::GLuint matricesBindingIndex) {
	m_program = new glow::Program;
	m_program->attach(replacePlaceholderAndGetShader(gl::GL_VERTEX_SHADER, vertShader), glow::Shader::fromString(gl::GL_FRAGMENT_SHADER, fragShader));

	setMatricesBlockBinding(matricesBindingIndex);
}

void glat::AbstractDrawingPrimitive::setupShader(const char* vertShader, const char* geomShader, const char* fragShader, gl::GLuint matricesBindingIndex) {
	m_program = new glow::Program;
	m_program->attach(replacePlaceholderAndGetShader(gl::GL_VERTEX_SHADER, vertShader),
		replacePlaceholderAndGetShader(gl::GL_GEOMETRY_SHADER, geomShader),
		glow::Shader::fromString(gl::GL_FRAGMENT_SHADER, fragShader)
	);

	setMatricesBlockBinding(matricesBindingIndex);
}

void glat::AbstractDrawingPrimitive::setMatricesBlockBinding(gl::GLuint bindingIndex) {
	// we need to link before binding uniform blocks
	m_program->link();

	if (m_program->getUniformBlockIndex("GlobalMatrices") != gl::GL_INVALID_INDEX) {
		m_program->uniformBlock("GlobalMatrices")->setBinding(bindingIndex);
	}
}

glow::Shader* glat::AbstractDrawingPrimitive::replacePlaceholderAndGetShader(gl::GLenum shaderType, const char* shader) {
	glowutils::StringTemplate* shaderSource = new glowutils::StringTemplate(new glow::StaticStringSource(shader, strlen(shader)));
	shaderSource->replace("### MATRIX_BLOCK ###", glat::ShaderSources::matrixUniformBlock);

	return new glow::Shader(shaderType, shaderSource);
}
