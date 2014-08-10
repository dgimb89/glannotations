#include <glbinding/gl/values.h>
#include <glowutils/StringTemplate.h>
#include <glowbase/StaticStringSource.h>

#include <glat/DrawingPrimitiveBase.h>
#include "ShaderSources.hpp"

glat::DrawingPrimitiveBase::DrawingPrimitiveBase() {
	m_vao = new glow::VertexArrayObject();
	m_positions = new glow::Buffer();
}

void glat::DrawingPrimitiveBase::setColor(glm::vec4 color) {
	m_program->setUniform("color", color);
}

glat::DrawingPrimitiveBase::~DrawingPrimitiveBase() {
	m_program->release();
}

void glat::DrawingPrimitiveBase::setupShader(const char* vertShader, const char* fragShader) {
	m_program = new glow::Program;
	m_program->attach(replacePlaceholderAndGetShader(gl::GL_VERTEX_SHADER, vertShader), glow::Shader::fromString(gl::GL_FRAGMENT_SHADER, fragShader));
}

void glat::DrawingPrimitiveBase::setupShader(const char* vertShader, const char* geomShader, const char* fragShader) {
	m_program = new glow::Program;
	m_program->attach(replacePlaceholderAndGetShader(gl::GL_VERTEX_SHADER, vertShader),
		replacePlaceholderAndGetShader(gl::GL_GEOMETRY_SHADER, geomShader),
		glow::Shader::fromString(gl::GL_FRAGMENT_SHADER, fragShader)
		);
}

void glat::DrawingPrimitiveBase::setBindingIndex(unsigned int bindingIndex) {
	// we need to link before binding uniform blocks
	m_program->link();

	if (m_program->getUniformBlockIndex("GlobalMatrices") != gl::GL_INVALID_INDEX) {
		m_program->uniformBlock("GlobalMatrices")->setBinding(bindingIndex);
	}
}

glow::Shader* glat::DrawingPrimitiveBase::replacePlaceholderAndGetShader(gl::GLenum shaderType, const char* shader) {
	glowutils::StringTemplate* shaderSource = new glowutils::StringTemplate(new glow::StaticStringSource(shader, strlen(shader)));
	shaderSource->replace("### MATRIX_BLOCK ###", glat::ShaderSources::matrixUniformBlock);

	return new glow::Shader(shaderType, shaderSource);
}
