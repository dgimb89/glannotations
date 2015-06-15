#include <glbinding/gl/values.h>
#include <glbinding/gl/enum.h>

#include <globjects/base/StringTemplate.h>
#include <globjects/base/StaticStringSource.h>
#include <glannotations/Renderer/DrawingPrimitiveBase.h>
#include "../ShaderSources.hpp"
#include <string.h>

glannotations::DrawingPrimitiveBase::DrawingPrimitiveBase() {
	initialize();
}

void glannotations::DrawingPrimitiveBase::setColor(glm::vec4 color) {
	m_program->setUniform("color", color);
}

glannotations::DrawingPrimitiveBase::~DrawingPrimitiveBase() {
	m_program->release();
}

void glannotations::DrawingPrimitiveBase::setupShader(const std::string & vertShader, const std::string & geomShader, const std::string & fragShader) {
	m_program = new globjects::Program;
	m_program->attach(	globjects::Shader::fromString(gl::GL_VERTEX_SHADER, vertShader), 
						finalizeGeometryShader(geomShader),
						globjects::Shader::fromString(gl::GL_FRAGMENT_SHADER, fragShader));
}

void glannotations::DrawingPrimitiveBase::setBindingIndex(unsigned int bindingIndex) {
	// we need to link before binding uniform blocks
	m_program->link();

	if (m_program->getUniformBlockIndex("GlobalMatrices") != gl::GL_INVALID_INDEX) {
		m_program->uniformBlock("GlobalMatrices")->setBinding(bindingIndex);
	}
}

globjects::Shader* glannotations::DrawingPrimitiveBase::finalizeGeometryShader(const std::string & shader) {
    globjects::StringTemplate* shaderSource = new globjects::StringTemplate(new globjects::StaticStringSource(shader));
	shaderSource->replace("### MATRIX_BLOCK ###", glannotations::ShaderSources::matrixUniformBlock);
	return new globjects::Shader(gl::GL_GEOMETRY_SHADER, shaderSource);
}

void glannotations::DrawingPrimitiveBase::initialize() {
	m_vao = new globjects::VertexArray();
	m_positions = new globjects::Buffer();
}
