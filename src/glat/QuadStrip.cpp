#include <glat/QuadStrip.h>
#include <glow/VertexAttributeBinding.h>

glat::QuadStrip::QuadStrip(std::shared_ptr<glow::Texture> distanceField) : glat::AbstractDrawingPrimitive(distanceField) {
	// initial position
	m_llf = m_urb = glm::vec3(0.0f, 0.0f, 0.0f);
	m_vertexCount = 0;
	m_secondTexCoords = new glow::Buffer(GL_ARRAY_BUFFER);
	m_texSwitch = new glow::Buffer(GL_ARRAY_BUFFER);
	// initial texture coordinates
	
}

void glat::QuadStrip::addQuad(glm::vec2 texture_ll, glm::vec2 texture_ur) {
	m_textureRanges.push_back(std::make_pair(texture_ll, texture_ur));
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
	m_vao->drawArrays(GL_QUAD_STRIP, 0, m_vertexCount);
	m_program->release();

	if (m_texture)
		m_texture->unbind();
}

void glat::QuadStrip::updateQuadRanges() {
	// update texture VBO
	std::vector<glm::vec2> firstTextureVector;
	std::vector<glm::vec2> secondTextureVector;
	std::vector<float> textureSwitch;

	// first two textureRanges in second vector does not matter but must be present for correct interpolation
	secondTextureVector.push_back(glm::vec2(0.f, 0.f));
	secondTextureVector.push_back(glm::vec2(0.f, 0.f));

	unsigned i = 0;
	double textureWidth = 0.0;
	for (auto textureCoords : m_textureRanges) {
		// calculate full texture width for later usage
		textureWidth += textureCoords.second.x - textureCoords.first.x;
		if (i % 2 == 0) {
			pushTextureCoords(firstTextureVector, textureCoords);
		}
		else {
			pushTextureCoords(secondTextureVector, textureCoords);
		}
		i++;
	}

	// last two textureRanges in first vector does not matter but must be present for correct interpolation
	firstTextureVector.push_back(glm::vec2(0.f, 0.f));
	firstTextureVector.push_back(glm::vec2(0.f, 0.f));

	// we build our texture switch VBO here
	for (i = 0; i < (2 * m_textureRanges.size()) + 2; i+=2) {
		if ((i / 4) % 2 == 0) {
			textureSwitch.push_back(-1.f);
			textureSwitch.push_back(-1.f);
		}
		else {
			textureSwitch.push_back(1.f);
			textureSwitch.push_back(1.f);
		}
	}

	m_texCoords->setData(firstTextureVector, GL_STATIC_DRAW);
	m_vao->binding(1)->setAttribute(1);
	m_vao->binding(1)->setFormat(firstTextureVector.size(), GL_FLOAT, GL_FALSE, 0);
	m_vao->binding(1)->setBuffer(m_texCoords, 0, sizeof(glm::vec2));
	m_vao->enable(1);

	m_secondTexCoords->setData(secondTextureVector, GL_STATIC_DRAW);
	m_vao->binding(2)->setAttribute(2);
	m_vao->binding(2)->setFormat(secondTextureVector.size(), GL_FLOAT, GL_FALSE, 0);
	m_vao->binding(2)->setBuffer(m_texCoords, 0, sizeof(glm::vec2));
	m_vao->enable(2);

	m_texSwitch->setData(textureSwitch, GL_STATIC_DRAW);
	m_vao->binding(3)->setAttribute(3);
	m_vao->binding(3)->setFormat(textureSwitch.size(), GL_FLOAT, GL_FALSE, 0);
	m_vao->binding(3)->setBuffer(m_texCoords, 0, sizeof(float));
	m_vao->enable(3);

	// update vertices VBO
	std::vector<glm::vec3> vertexVector;

	glm::vec3 spanVector = m_urb - m_llf;
	// normalize
	spanVector /= textureWidth;

	double heightDiff = m_urb.y - m_llf.y;

	for (auto textureCoords : m_textureRanges) {
		//auto test = m_llf + glm::vec;
		//heightDiff
		//vertexVector.push_back(m_llf + ((textureCoords.second.x) / textureWidth * normSpanVector));
	}

	m_positions->setData(vertexVector, GL_STATIC_DRAW);

	m_vao->binding(0)->setAttribute(0);
	m_vao->binding(0)->setFormat(vertexVector.size(), GL_FLOAT, GL_FALSE, 0);
	m_vao->binding(0)->setBuffer(m_positions, 0, sizeof(glm::vec4));
	m_vao->enable(0);

	m_vertexCount = vertexVector.size();
}

void glat::QuadStrip::setPosition(glm::vec3 llf, glm::vec3 urb, glm::mat4 modelViewProjection /*= glm::mat4()*/) {
	m_llf = llf;
	m_urb = urb;
	m_program->setUniform("modelViewProjection", modelViewProjection);

	updateQuadRanges();
}

glm::vec2 glat::QuadStrip::getUL(const textureRange_t& textureRange) {
	return glm::vec2(textureRange.first.x, textureRange.second.y);
}

glm::vec2 glat::QuadStrip::getLR(const textureRange_t& textureRange) {
	return glm::vec2(textureRange.second.x, textureRange.first.y);
}

void glat::QuadStrip::pushTextureCoords(std::vector<glm::vec2>& textureVec, const textureRange_t& textureRange) {
	textureVec.push_back(textureRange.first);
	textureVec.push_back(getUL(textureRange));
	textureVec.push_back(getLR(textureRange));
	textureVec.push_back(textureRange.second);
}
