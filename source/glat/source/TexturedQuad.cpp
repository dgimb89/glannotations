#include <glat/TexturedQuad.h>
#include <glow/VertexAttributeBinding.h>


static const char*	vertQuadShaderSource = R"(
				#version 330

				uniform mat4 modelViewProjection;

				layout (location = 0) in vec3 position;
				layout (location = 1) in vec2 textureCoord;
				out vec2 v_uv;

				void main()
				{
					v_uv = textureCoord;
					gl_Position = modelViewProjection * vec4(position, 1.0);
				}
				)";

static const char* fragQuadShaderSource = R"(
				#version 330

				uniform sampler2D source;

				layout (location = 0) out vec4 fragColor;

				in vec2 v_uv;

				void main()
				{
					fragColor = texture2D(source, v_uv);
				}
				)";

static const char* fragDFQuadShaderSource = R"(
				#version 330

				uniform sampler2D source;
				uniform int style;
				uniform vec4 color;
				uniform vec3 outlineColor;
				uniform float outlineSize;
				uniform float bumpIntensity;

				layout (location = 0) out vec4 fragColor;

				in vec2 v_uv;

				vec4 getText() {
					float d = texture2D(source, v_uv).x - 0.5;	

					if (d < 0.0) {
						return vec4(color.rgb, color.a * smoothstep(0.0, 0.03, abs(d)));
					} 
					else {
						discard;
					}
				}

				vec4 getTextWithOutline() {
					float d = texture2D(source, v_uv).x - 0.48;	

					// Interpolations Faktor zwischen outline und Welt
					float d_outline = smoothstep(outlineSize, 0.00 , d);

					if (d < 0.0) {
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
					float dx = dFdx(texture2D(source, v_uv).x) * 45.0 * bumpIntensity;
					float dy = dFdy(texture2D(source, v_uv).x) * 50.0 * bumpIntensity;

					vec3 vx = vec3(1.0, 0.0, dx);
					vec3 vy = vec3(0.0, 1.0, dy);

					vec3 n = cross(vx, vy);
					vec3 lightSource = vec3(1.5, 1.5, 3.0);

					n = normalize(n);
					lightSource = normalize(lightSource);

					return dot(n, lightSource);
				}

				void main()
				{
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

glat::TexturedQuad::TexturedQuad(std::shared_ptr<glow::Texture> texture, bool isDistanceField) : glat::AbstractTexturedPrimitive(texture) {
	m_style = 0;
	if (isDistanceField) {
		setupShader(fragDFQuadShaderSource, vertQuadShaderSource);
	}
	else {
		setupShader(fragQuadShaderSource, vertQuadShaderSource);
	}
	// Position
	m_vao->binding(0)->setAttribute(0);
	m_vao->binding(0)->setFormat(3, GL_FLOAT, GL_FALSE, 0);
	m_vao->enable(0);

	// Texture Coordinates
	std::array<glm::vec2, 4> textureArray{ {
		glm::vec2(1.f, 0.f)
		, glm::vec2(1.f, 1.f)
		, glm::vec2(0.f, 0.f)
		, glm::vec2(0.f, 1.f)
	} };

	m_texCoords->setData(textureArray, GL_STATIC_DRAW);

	m_vao->binding(1)->setAttribute(1);
	m_vao->binding(1)->setBuffer(m_texCoords, 0, sizeof(glm::vec2));
	m_vao->binding(1)->setFormat(2, GL_FLOAT, GL_FALSE, 0);
	m_vao->enable(1);

	setPosition(glm::vec3(-1.f, -1.f, 0.f), glm::vec3(1.f, -1.f, 0.f), glm::vec3(1.f, 1.f, 0.f));

	m_program->setUniform("source", 0);
}


void glat::TexturedQuad::setPosition(glm::vec3 ll, glm::vec3 lr, glm::vec3 ur, glm::mat4 modelViewProjection /*= glm::mat4()*/) {

	std::array<glm::vec3, 4> vertexArray{ {
				glm::vec3(lr.x, lr.y, lr.z)
			,	glm::vec3(ur.x, ur.y, ur.z)
			,	glm::vec3(ll.x, ll.y, ll.z)
			,	glm::vec3(ll.x + (ur.x - lr.x), ll.y + (ur.y - lr.y), ll.z + (ur.z - lr.z))

	}};


	m_positions->setData(vertexArray, GL_STATIC_DRAW);
	m_vao->binding(0)->setBuffer(m_positions, 0, sizeof(glm::vec3));

	setModelViewProjection(modelViewProjection);
}


void glat::TexturedQuad::draw() {
	m_program->release();
	if (m_texture)
	{
		glActiveTexture(GL_TEXTURE0);
		CheckGLError();

		m_texture->bind();
	}

	m_program->use();
	m_vao->drawArrays(GL_TRIANGLE_STRIP, 0, 4);
	m_program->release();

	if (m_texture)
		m_texture->unbind();
}