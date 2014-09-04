#include <glbinding/gl/values.h>
#include <globjects-utils/StringTemplate.h>
#include <globjects-base/StaticStringSource.h>

#include <glat/DrawingPrimitiveBase.h>
#include "ShaderSources.hpp"
#include <string.h>
glat::DrawingPrimitiveBase::DrawingPrimitiveBase() {
	m_vao = new glo::VertexArrayObject();
	m_positions = new glo::Buffer();
}

void glat::DrawingPrimitiveBase::setColor(glm::vec4 color) {
	m_program->setUniform("color", color);
}

glat::DrawingPrimitiveBase::~DrawingPrimitiveBase() {
	m_program->release();
}

void glat::DrawingPrimitiveBase::setupShader(const char* vertShader, const char* geomShader, const char* fragShader) {
	m_program = new glo::Program;
	m_program->attach(	glo::Shader::fromString(gl::GL_VERTEX_SHADER, vertShader), 
						finalizeGeometryShader(geomShader),
						glo::Shader::fromString(gl::GL_FRAGMENT_SHADER, fragShader));
}

void glat::DrawingPrimitiveBase::setBindingIndex(unsigned int bindingIndex) {
	// we need to link before binding uniform blocks
	m_program->link();

	if (m_program->getUniformBlockIndex("GlobalMatrices") != gl::GL_INVALID_INDEX) {
		m_program->uniformBlock("GlobalMatrices")->setBinding(bindingIndex);
	}
}

glo::Shader* glat::DrawingPrimitiveBase::finalizeGeometryShader(const char* shader) {
	gloutils::StringTemplate* shaderSource = new gloutils::StringTemplate(new glo::StaticStringSource(shader, strlen(shader)));
	shaderSource->replace("### MATRIX_BLOCK ###", glat::ShaderSources::matrixUniformBlock);
	return new glo::Shader(gl::GL_GEOMETRY_SHADER, shaderSource);
}
