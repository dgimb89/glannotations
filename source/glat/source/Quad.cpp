#include <glat/Quad.h>
#include <glow/VertexAttributeBinding.h>

static const char*	vertQuadShaderSource = R"(
				#version 330

				uniform mat4 modelViewProjection;

				layout (location = 0) in vec3 position;

				void main()
				{
					gl_Position = modelViewProjection * vec4(position, 1.0);
				}
				)";

static const char* fragQuadShaderSource = R"(
				#version 330
				uniform vec4 color;

				layout (location = 0) out vec4 fragColor;

				void main()
				{
					fragColor = color;
				}
				)";

void glat::Quad::draw() {
	m_program->release();
	m_program->use();
	m_vao->drawArrays(GL_TRIANGLE_STRIP, 0, 4);
	m_program->release();
}

void glat::Quad::setPosition(glm::vec3 ll, glm::vec3 lr, glm::vec3 ur, glm::mat4 modelViewProjection /*= glm::mat4()*/) {
	std::array<glm::vec3, 4> vertexArray{ {
		glm::vec3(lr.x, lr.y, lr.z)
		, glm::vec3(ur.x, ur.y, ur.z)
		, glm::vec3(ll.x, ll.y, ll.z)
		, glm::vec3(ll.x + (ur.x - lr.x), ll.y + (ur.y - lr.y), ll.z + (ur.z - lr.z))

	} };


	m_positions->setData(vertexArray, GL_STATIC_DRAW);
	m_vao->binding(0)->setBuffer(m_positions, 0, sizeof(glm::vec3));

	setModelViewProjection(modelViewProjection);
}

glat::Quad::Quad() {
	setupShader(fragQuadShaderSource, vertQuadShaderSource);
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

	setPosition(glm::vec3(-1.f, -1.f, 0.f), glm::vec3(1.f, -1.f, 0.f), glm::vec3(1.f, 1.f, 0.f));
}

