#include <glat/QuadStrip.h>
#include "ShaderSources.hpp"

#include <globjects/VertexAttributeBinding.h>
#include <algorithm>

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
	uniform bool onNearplane;

	### MATRIX_BLOCK ###

	layout(points) in;
	layout(triangle_strip, max_vertices = 4) out;

	in QuadData {
		vec2 texCoord;
		vec2 texAdvance;
		vec3 advanceH;
		vec3 advanceW;
	} quad[];

	out vec2 v_uv;

	void setVertexData(in float upper, in float right, out vec4 position, out vec2 texCoord) {
		position = vec4(gl_in[0].gl_Position.xyz + upper * quad[0].advanceH + right * quad[0].advanceW, 1.0);
		texCoord = quad[0].texCoord + vec2(right * quad[0].texAdvance.x, upper * quad[0].texAdvance.y);
	}

	void main() {
		mat4 viewProjection = (1.0 - float(onNearplane)) * (projectionMatrix * viewMatrix)  +  float(onNearplane) * mat4(1);

		// ll
		setVertexData(0.0, 0.0, gl_Position, v_uv);
		gl_Position = viewProjection * gl_Position;
		EmitVertex();

		// lr
		setVertexData(0.0, 1.0, gl_Position, v_uv);
		gl_Position = viewProjection * gl_Position;
		EmitVertex();

		// ul
		setVertexData(1.0, 0.0, gl_Position, v_uv);
		gl_Position = viewProjection * gl_Position;
		EmitVertex();

		// ur
		setVertexData(1.0, 1.0, gl_Position, v_uv);
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

		in vec2 v_uv;

		float aastep(float t, float value)  {
			//float afwidth = length(vec2(dFdx(value), dFdy(value))) * 1.0;
			float afwidth = fwidth(value);
			return smoothstep(t - afwidth, t + afwidth, value);
		}

		float tex(float t, vec2 uv) {
			return aastep(t, texture(source, uv).r);
		}

		float aastep3(float t, vec2 uv) {
			vec2 dfdy = dFdy(uv);

			vec2 y = dfdy * 1.0 / 3.0;  

			float v = tex(t, uv - y)    ;
				  v = tex(t, uv    ) + v;
				  v = tex(t, uv + y) + v;

			return v / 3.0;
		}

		float aastep8(float t, vec2 uv) {
			vec2 dfdx = dFdx(uv);
			vec2 dfdy = dFdy(uv);

			vec2 x1 = dfdx * 1.0 / 8.0;
			vec2 y1 = dfdy * 1.0 / 8.0;  
			vec2 y2 = dfdy * 3.0 / 8.0;  

			float v = tex(t, uv - x1 - y2)    ;
				  v = tex(t, uv - x1 - y1) + v;
				  v = tex(t, uv - x1 + y1) + v;
				  v = tex(t, uv - x1 + y2) + v;

				  v = tex(t, uv + x1 - y2) + v;
				  v = tex(t, uv + x1 - y1) + v;
				  v = tex(t, uv + x1 + y1) + v;
				  v = tex(t, uv + x1 + y2) + v;

			return v / 8.0;
		}

		float aastep12(float t, vec2 uv) {
			vec2 dfdx = dFdx(uv);
			vec2 dfdy = dFdy(uv);

			vec2 x1 = dfdx * 1.0 / 12.0;
			vec2 y1 = dfdy * 1.0 / 12.0;  
			vec2 y2 = dfdy * 3.0 / 12.0;  
			vec2 y3 = dfdy * 5.0 / 12.0;  

			float v = tex(t, uv - x1 - y3)    ;
				  v = tex(t, uv - x1 - y2) + v;
				  v = tex(t, uv - x1 - y1) + v;
				  v = tex(t, uv - x1 + y1) + v;
				  v = tex(t, uv - x1 + y2) + v;
				  v = tex(t, uv - x1 + y3) + v;

				  v = tex(t, uv + x1 - y3) + v;
				  v = tex(t, uv + x1 - y2) + v;
				  v = tex(t, uv + x1 - y1) + v;
				  v = tex(t, uv + x1 + y1) + v;
				  v = tex(t, uv + x1 + y2) + v;
				  v = tex(t, uv + x1 + y3) + v;

			return v / 12.0;
		}

		float aastep3x3(float t, vec2 uv) {
			vec2 dfdx = dFdx(uv);
			vec2 dfdy = dFdy(uv);

			vec2 x = dfdx * 1.0 / 3.0;
			vec2 y = dfdy * 1.0 / 3.0;  

			float v = tex(t, uv - x - y)    ;
				  v = tex(t, uv - x    ) + v;
				  v = tex(t, uv - x + y) + v;

				  v = tex(t, uv     - y) + v;
				  v = tex(t, uv        ) + v;
				  v = tex(t, uv     + y) + v;

				  v = tex(t, uv + x - y) + v;
				  v = tex(t, uv + x    ) + v;
				  v = tex(t, uv + x + y) + v;

			return v / 9.0;
		}

		float aastep4x4s(float t, vec2 uv) {
			vec2 dfdx = dFdx(uv);
			vec2 dfdy = dFdy(uv);

			vec2 x1 = dfdx * 1.0 / 8.0;
			vec2 y1 = dfdy * 1.0 / 8.0;
			vec2 x2 = dfdx * 3.0 / 8.0;
			vec2 y2 = dfdy * 3.0 / 8.0;  

			float v = tex(t, uv - x2 - y2)    ;
				  v = tex(t, uv - x2 - y1) + v;
				  v = tex(t, uv - x2 + y1) + v;
				  v = tex(t, uv - x2 + y2) + v;

				  v = tex(t, uv - x1 - y2) + v;
				  v = tex(t, uv - x1 - y1) + v;
				  v = tex(t, uv - x1 + y1) + v;
				  v = tex(t, uv - x1 + y2) + v;

				  v = tex(t, uv + x1 - y2) + v;
				  v = tex(t, uv + x1 - y1) + v;
				  v = tex(t, uv + x1 + y1) + v;
				  v = tex(t, uv + x1 + y2) + v;

				  v = tex(t, uv + x2 - y2) + v;
				  v = tex(t, uv + x2 - y1) + v;
				  v = tex(t, uv + x2 + y1) + v;
				  v = tex(t, uv + x2 + y2) + v;

			return v / 16.0;
		}

		vec4 subpix(float r, float g, float b, vec4 fore, vec4 back) {
			return vec4(mix(back.rgb, fore.rgb, vec3(r, g, b)), mix(back.a, fore.a, (r + b + g) / 3.0));
		}

		void main() {
			float s = texture(source, v_uv).r;
			if(s > 0.6)
				discard;

			vec4 fc = vec4(color.rgb, 0.0);
			vec4 bc = color;

			float threshold = 0.6;

			// subpixel variations

			float r, g, b;

			vec2 dfdx = dFdx(v_uv);
			vec2 dfdy = dFdy(v_uv);
	
			vec2 uvr = v_uv;
			vec2 uvg = v_uv + dfdx * 1.0 / 3.0;
			vec2 uvb = v_uv + dfdx * 2.0 / 3.0;

			if(true)
			//if(x > 0.002) // optional: only use if glyph is small (todo: adjust threshold)
			{	// higher quality - more samples
				r = aastep12(threshold, uvr);
				g = aastep12(threshold, uvg);
				b = aastep12(threshold, uvb);
			}
			else
			{	// lower quality - less samples
				r = aastep3(threshold, uvr);
				g = aastep3(threshold, uvg);
				b = aastep3(threshold, uvb);
			}
			fragColor = subpix(r, g, b, fc, bc);
		}
	)";

		static const char* texturingFragShader = R"(
	#version 330
	uniform sampler2D source;

	layout (location = 0) out vec4 fragColor;
	in vec2 v_uv;

	void main() {
		fragColor = texture2D(source, v_uv);
		if(fragColor.a < 0.001) discard;
	}

	)";

glat::QuadStrip::QuadStrip(std::shared_ptr<glo::Texture> texture, gl::GLuint matricesBindingIndex, bool isDistanceField) : glat::AbstractTexturedPrimitive(texture) {
	if (isDistanceField) {
		setupShader(vertShader, geomShader, dfFragShader);
	} else {
		setupShader(vertShader, geomShader, texturingFragShader);
	}
	setBindingIndex(matricesBindingIndex);

	// initial position
	m_ll = m_lr = m_ur = glm::vec3(0.0f, 0.0f, 0.0f);
	m_vertexCount = 0;
	m_advanceH = new glo::Buffer;
	m_advanceW = new glo::Buffer;
	m_texAdvance = new glo::Buffer;

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

bool glat::QuadStrip::setPosition(glm::vec3 ll, glm::vec3 lr, glm::vec3 ur) {
	if (positionValid(ll, lr, ur)) {
		m_ll = ll;
		m_lr = lr;
		m_ur = ur;
		updateQuadRanges();
		// finalize geom shader for internal rendering
		m_program->setUniform("onNearplane", false);
		return true;
	}
	return false;
}

bool glat::QuadStrip::setViewportPosition(glm::vec2 ll, glm::vec2 lr, glm::vec2 ur) {
	if (setPosition(glm::vec3(ll, 0.f), glm::vec3(lr, 0.f), glm::vec3(ur, 0.f))) {
		// finalize geom shader for viewport rendering
		m_program->setUniform("onNearplane", true);
		return true;
	}
	return false;
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

bool glat::QuadStrip::positionValid( const glm::vec3& ll, const glm::vec3& lr, const glm::vec3& ur ) const {
	return ll != lr && ll != ur && lr != ur;
}