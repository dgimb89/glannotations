#include <glat/QuadStrip.h>
#include <glow/VertexAttributeBinding.h>
#include "ShaderSources.hpp"

glat::QuadStrip::QuadStrip(std::shared_ptr<glow::Texture> distanceField) : glat::AbstractDFPrimitive(distanceField) {
	setupShader(ShaderSource::fragQuadStripShaderSource, ShaderSource::vertQuadStripShaderSource);
	// initial position
	m_ll = m_lr = m_ur = glm::vec3(0.0f, 0.0f, 0.0f);
	m_vertexCount = 0;
	m_advanceH = new glow::Buffer(GL_ARRAY_BUFFER);
	m_advanceW = new glow::Buffer(GL_ARRAY_BUFFER);
	m_texAdvance = new glow::Buffer(GL_ARRAY_BUFFER);


	m_geometryShader = glow::Shader::fromString(GL_GEOMETRY_SHADER, ShaderSource::geomQuadStripShaderSource);
	m_program->attach(m_geometryShader);

	m_vao->binding(0)->setAttribute(0);
	m_vao->binding(0)->setFormat(3, GL_FLOAT, GL_FALSE, 0);
	m_vao->enable(0);
}

void glat::QuadStrip::addQuad(texVec2_t texture_ll, texVec2_t texture_advance) {
	m_textureRanges.push_back(std::make_pair(texture_ll, texture_advance));
}

void glat::QuadStrip::clearQuads() {
	m_textureRanges.clear();
}

void glat::QuadStrip::draw() {
	m_program->release();
	if (m_texture)
	{
		glActiveTexture(GL_TEXTURE0);
		CheckGLError();

		m_texture->bind();
	}

	m_program->use();
	m_vao->drawArrays(GL_POINTS, 0, m_vertexCount);
	m_program->release();

	if (m_texture)
		m_texture->unbind();
}

void glat::QuadStrip::updateQuadRanges() {
	// update texture VBO
	std::vector<texVec2_t> textures, texAdvances;

	double textureWidth = 0.0;
	for (auto textureCoords : m_textureRanges) {
		// calculate full texture width for later usage
		textureWidth += textureCoords.second.x;
		textures.push_back(textureCoords.first);
		texAdvances.push_back(textureCoords.second);
	}

	// update vertices VBO
	std::vector<glm::vec3> vertexVector;
	std::vector<glm::vec3> vertAdvanceW, vertAdvanceH;

	glm::vec3 widthSpan = m_lr - m_ll;
	widthSpan /= textureWidth; // normalize
	glm::vec3 heightSpan = m_ur - m_lr;
	glm::vec3 currentLL = m_ll;

	for (auto textureCoords : m_textureRanges) {
		// we can calculate necessary width for current quad from texture widths because the scaling is done uniformly
		vertexVector.push_back(currentLL);
		auto currentWidthSpan = widthSpan;
		currentWidthSpan *= textureCoords.second.x;
		vertAdvanceW.push_back(currentWidthSpan);
		vertAdvanceH.push_back(heightSpan);

		currentLL += currentWidthSpan;
	}

	m_positions->setData(vertexVector, GL_STATIC_DRAW);
	m_vao->binding(0)->setAttribute(0);
	m_vao->binding(0)->setFormat(3, GL_FLOAT, GL_FALSE, 0);
	m_vao->binding(0)->setBuffer(m_positions, 0, sizeof(glm::vec3));
	m_vao->enable(0);
	m_vertexCount = vertexVector.size();

	m_texCoords->setData(textures, GL_STATIC_DRAW);
	m_vao->binding(1)->setAttribute(1);
	m_vao->binding(1)->setFormat(2, GL_FLOAT, GL_FALSE, 0);
	m_vao->binding(1)->setBuffer(m_texCoords, 0, sizeof(texVec2_t));
	m_vao->enable(1);

	m_texAdvance->setData(texAdvances, GL_STATIC_DRAW);
	m_vao->binding(2)->setAttribute(2);
	m_vao->binding(2)->setFormat(2, GL_FLOAT, GL_FALSE, 0);
	m_vao->binding(2)->setBuffer(m_texAdvance, 0, sizeof(texVec2_t));
	m_vao->enable(2);

	m_advanceH->setData(vertAdvanceH, GL_STATIC_DRAW);
	m_vao->binding(3)->setAttribute(3);
	m_vao->binding(3)->setFormat(3, GL_FLOAT, GL_FALSE, 0);
	m_vao->binding(3)->setBuffer(m_advanceH, 0, sizeof(glm::vec3));
	m_vao->enable(3);

	m_advanceW->setData(vertAdvanceW, GL_STATIC_DRAW);
	m_vao->binding(4)->setAttribute(4);
	m_vao->binding(4)->setFormat(3, GL_FLOAT, GL_FALSE, 0);
	m_vao->binding(4)->setBuffer(m_advanceW, 0, sizeof(glm::vec3));
	m_vao->enable(4);
}

void glat::QuadStrip::setPosition(glm::vec3 ll, glm::vec3 lr, glm::vec3 ur, glm::mat4 modelViewProjection /*= glm::mat4()*/) {
	if (ll != m_ll || lr != m_lr || ur != m_ur) {
		m_ll = ll;
		m_lr = lr;
		m_ur = ur;
		updateQuadRanges();
	}
	setModelViewProjection(modelViewProjection);
}

glat::QuadStrip::texVec2_t glat::QuadStrip::getUL(const textureRange_t& textureRange) {
	return texVec2_t(textureRange.first.x, textureRange.second.y);
}

glat::QuadStrip::texVec2_t glat::QuadStrip::getLR(const textureRange_t& textureRange) {
	return texVec2_t(textureRange.second.x, textureRange.first.y);
}

void glat::QuadStrip::pushTextureCoords(std::vector<texVec2_t>& textureVec, const textureRange_t& textureRange) {
	textureVec.push_back(textureRange.first);
	textureVec.push_back(getUL(textureRange));
	textureVec.push_back(getLR(textureRange));
	textureVec.push_back(textureRange.second);
}
