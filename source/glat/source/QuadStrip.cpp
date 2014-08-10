#include <glat/QuadStrip.h>
#include <glow/VertexAttributeBinding.h>

static const char*	vertShader = R"(
	#version 330

	layout (location = 0) in vec3 position;
	layout (location = 1) in vec2 texCoord;
	layout (location = 2) in vec2 texAdvance;
	layout (location = 3) in vec3 advanceH;
	layout (location = 4) in vec3 advanceW;
				
	out QuadData {
		vec2 texCoord;
		vec2 texAdvance;
		vec3 advanceH;
		vec3 advanceW;
	} quad;

	void main()
	{
		quad.texCoord = texCoord;
		quad.advanceH = advanceH;
		quad.advanceW = advanceW;
		quad.texAdvance = texAdvance;
		gl_Position = vec4(position, 1.0);
	}
	)";

static const char* geomShader = R"(
	#version 330
	### MATRIX_BLOCK ###

	layout(points) in;
	layout(triangle_strip, max_vertices = 4) out;

	in QuadData {
		vec2 texCoord;
		vec2 texAdvance;
		vec3 advanceH;
		vec3 advanceW;
	} quad[];

	out VertexData {
		vec2 texCoord;
	} vertex;

	void setVertexData(in float upper, in float right, out vec4 position, out vec2 texCoord) {
		position = vec4(gl_in[0].gl_Position.xyz + upper * quad[0].advanceH + right * quad[0].advanceW, 1.0);
		texCoord = quad[0].texCoord + vec2(right * quad[0].texAdvance.x, upper * quad[0].texAdvance.y);
	}

	void main() {
		mat4 viewProjection = projectionMatrix * viewMatrix;
		// ll
		setVertexData(0.0, 0.0, gl_Position, vertex.texCoord);
		gl_Position = viewProjection * gl_Position;
		EmitVertex();

		// ul
		setVertexData(1.0, 0.0, gl_Position, vertex.texCoord);
		gl_Position = viewProjection * gl_Position;
		EmitVertex();

		// lr
		setVertexData(0.0, 1.0, gl_Position, vertex.texCoord);
		gl_Position = viewProjection * gl_Position;
		EmitVertex();

		// ur
		setVertexData(1.0, 1.0, gl_Position, vertex.texCoord);
		gl_Position = viewProjection * gl_Position;
		EmitVertex();

		EndPrimitive();
	}
	)";

	static const char* dfFragShader = R"(
	#version 330
	uniform sampler2D source;
	uniform int style;
	uniform vec4 color;
	uniform vec3 outlineColor;
	uniform float outlineSize;
	uniform float bumpIntensity;

	layout (location = 0) out vec4 fragColor;

	in VertexData {
		vec2 texCoord;
	} vertex;

	vec4 getText() {
		float distance = texture2D(source, vertex.texCoord);
		if(distance > 0.5) {
			discard;
		}
		return vec4(color.rgb, color.a * (1.0 - smoothstep(0.49, 0.5, distance)));
	}

	vec4 getTextWithOutline() {
		float distance = texture2D(source, vertex.texCoord);

		// Interpolations Faktor zwischen outline und Welt
		float d_outline = smoothstep(outlineSize + 0.5, 0.5 , distance);

		if (distance < 0.5) {
			return color;
		}
		else if (d_outline > 0.0) {
			return vec4(outlineColor, d_outline);
		}
		else {
			return vec4(0.0, 0.0, 0.0, 0.0);
		}
	}

	float getBumpMapEffect() {
		float dx = dFdx(texture2D(source, vertex.texCoord).x) * 45.0 * bumpIntensity;
		float dy = dFdy(texture2D(source, vertex.texCoord).x) * 50.0 * bumpIntensity;

		vec3 vx = vec3(1.0, 0.0, dx);
		vec3 vy = vec3(0.0, 1.0, dy);

		vec3 n = cross(vx, vy);
		vec3 lightSource = vec3(1.5, 1.5, 3.0);

		n = normalize(n);
		lightSource = normalize(lightSource);

		return dot(n, lightSource);
	}

	void main() {
		if (style == 1) {
			fragColor = getTextWithOutline();
		} else if (style == 2) {
			vec4 text = getText();
			fragColor = vec4(text.rgb * getBumpMapEffect(), text.a);
		} else if (style == 3) {
			vec4 text = getTextWithOutline();
			fragColor = vec4(text.rgb * getBumpMapEffect(), text.a);
		} else {
			fragColor = getText();
		}
	}

	)";

static const char* texturingFragShader = R"(
	#version 330
	uniform sampler2D source;

	layout (location = 0) out vec4 fragColor;

	in VertexData {
		vec2 texCoord;
	} vertex;


	void main() {
		fragColor = texture2D(source, vertex.texCoord);
		if(fragColor.a < 0.001) discard;
	}

	)";

glat::QuadStrip::QuadStrip(std::shared_ptr<glow::Texture> texture, gl::GLuint matricesBindingIndex, bool isDistanceField) : glat::AbstractTexturedPrimitive(texture) {
	if (isDistanceField) {
		setupShader(vertShader, geomShader, dfFragShader);
		setBindingIndex(matricesBindingIndex);
	} else {
		setupShader(vertShader, geomShader, texturingFragShader);
		setBindingIndex(matricesBindingIndex);
	}
	// initial position
	m_ll = m_lr = m_ur = glm::vec3(0.0f, 0.0f, 0.0f);
	m_vertexCount = 0;
	m_advanceH = new glow::Buffer;
	m_advanceW = new glow::Buffer;
	m_texAdvance = new glow::Buffer;

	m_vao->binding(0)->setAttribute(0);
	m_vao->binding(0)->setFormat(3, gl::GL_FLOAT, gl::GL_FALSE, 0);
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
	if (m_texture) {
		gl::glActiveTexture(gl::GL_TEXTURE0);
		m_texture->bind();
	}

	m_program->use();
	m_vao->drawArrays(gl::GL_POINTS, 0, m_vertexCount);
	m_program->release();

	if (m_texture) {
		m_texture->unbind();
	}
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

	m_positions->setData(vertexVector, gl::GL_STATIC_DRAW);
	m_vao->binding(0)->setAttribute(0);
	m_vao->binding(0)->setFormat(3, gl::GL_FLOAT, gl::GL_FALSE, 0);
	m_vao->binding(0)->setBuffer(m_positions, 0, sizeof(glm::vec3));
	m_vao->enable(0);
	m_vertexCount = vertexVector.size();

	m_texCoords->setData(textures, gl::GL_STATIC_DRAW);
	m_vao->binding(1)->setAttribute(1);
	m_vao->binding(1)->setFormat(2, gl::GL_FLOAT, gl::GL_FALSE, 0);
	m_vao->binding(1)->setBuffer(m_texCoords, 0, sizeof(texVec2_t));
	m_vao->enable(1);

	m_texAdvance->setData(texAdvances, gl::GL_STATIC_DRAW);
	m_vao->binding(2)->setAttribute(2);
	m_vao->binding(2)->setFormat(2, gl::GL_FLOAT, gl::GL_FALSE, 0);
	m_vao->binding(2)->setBuffer(m_texAdvance, 0, sizeof(texVec2_t));
	m_vao->enable(2);

	m_advanceH->setData(vertAdvanceH, gl::GL_STATIC_DRAW);
	m_vao->binding(3)->setAttribute(3);
	m_vao->binding(3)->setFormat(3, gl::GL_FLOAT, gl::GL_FALSE, 0);
	m_vao->binding(3)->setBuffer(m_advanceH, 0, sizeof(glm::vec3));
	m_vao->enable(3);

	m_advanceW->setData(vertAdvanceW, gl::GL_STATIC_DRAW);
	m_vao->binding(4)->setAttribute(4);
	m_vao->binding(4)->setFormat(3, gl::GL_FLOAT, gl::GL_FALSE, 0);
	m_vao->binding(4)->setBuffer(m_advanceW, 0, sizeof(glm::vec3));
	m_vao->enable(4);
}

void glat::QuadStrip::setPosition(glm::vec3 ll, glm::vec3 lr, glm::vec3 ur) {
	if (ll != m_ll || lr != m_lr || ur != m_ur) {
		m_ll = ll;
		m_lr = lr;
		m_ur = ur;
		updateQuadRanges();
	}
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

float glat::QuadStrip::getUniformQuadHeight() {
	return m_textureRanges.front().second.y; // returning random texture advance y
}

size_t glat::QuadStrip::getQuadstripRowCount() {
	// TODO: support multiple rows
	return 1u;
}

float glat::QuadStrip::getQuadStripHeight() {
	return getQuadstripRowCount() * getUniformQuadHeight();
}

float glat::QuadStrip::getQuadStripWidth() {
	// TODO: adapt when multiple row support is added
	float resultWidth = 0.f;
	std::for_each(m_textureRanges.begin(), m_textureRanges.end(), [&](textureRange_t elem){ resultWidth += elem.second.x; });
	return resultWidth;
}
