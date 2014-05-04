#include <glat/Prismoid.h>
#include <glow/VertexAttributeBinding.h>

static const char*	vertPrismoidShaderSource = R"(
				#version 330

				uniform mat4 modelViewProjection;

				layout (location = 0) in vec3 position;

				out PrismoidData {
					mat4 mvp;
				} prism;

				void main() {
					prism.mvp = modelViewProjection;
					gl_Position = vec4(position, 1.0);
				}
				)";

static const char* geomPrismoidShaderSource = R"(
				#version 330

				layout(lines_adjacency) in;
				layout(triangle_strip, max_vertices = 24) out;

				in PrismoidData {
					mat4 mvp;
				} prism[];

				vec4 prismoid[8]; // Scratch space for the eight corners of the prismoid

				void emit(int a, int b, int c, int d) {
					gl_Position = prismoid[a]; EmitVertex();
					gl_Position = prismoid[b]; EmitVertex();
					gl_Position = prismoid[c]; EmitVertex();
					gl_Position = prismoid[d]; EmitVertex();
					EndPrimitive();
				}

				void main() {
					// Compute orientation vectors for the two connecting faces:
					vec3 p0, p1, p2, p3;
					p0 = gl_in[0].gl_Position.xyz; p1 = gl_in[1].gl_Position.xyz;
					p2 = gl_in[2].gl_Position.xyz; p3 = gl_in[3].gl_Position.xyz;
					vec3 n0 = normalize(p1 - p0);
					vec3 n1 = normalize(p2 - p1);
					vec3 n2 = normalize(p3 - p2);
					vec3 u = normalize(n0 + n1);
					vec3 v = normalize(n1 + n2);

					// Declare scratch variables for basis vectors:
					vec3 i, j, k; float r = 0.05;

					// Compute face 1 of 2:
					j = u; i = vec3(0,1,0); k = cross(i, j); i *= r; k *= r;
					prismoid[0] = prism[0].mvp * vec4(p1 + i + k, 1);
					prismoid[1] = prism[0].mvp * vec4(p1 + i - k, 1);
					prismoid[2] = prism[0].mvp * vec4(p1 - i - k, 1);
					prismoid[3] = prism[0].mvp * vec4(p1 - i + k, 1);

					// Compute face 2 of 2:
					j = v; i = vec3(0,1,0); k = cross(i, j); i *= r; k *= r;
					prismoid[4] = prism[0].mvp * vec4(p2 + i + k, 1);
					prismoid[5] = prism[0].mvp * vec4(p2 + i - k, 1);
					prismoid[6] = prism[0].mvp * vec4(p2 - i - k, 1);
					prismoid[7] = prism[0].mvp * vec4(p2 - i + k, 1);

					// Emit the six faces of the prismoid:
					emit(0, 1, 3, 2); emit(5, 4, 6, 7);
					emit(4, 5, 0, 1); emit(3, 2, 7, 6);
					emit(0, 3, 4, 7); emit(2, 1, 6, 5);
				}
				)";


static const char* fragPrismoidShaderSource = R"(
				#version 330
				uniform vec4 color;

				layout (location = 0) out vec4 fragColor;

				void main() {
					fragColor = color;
				}
				)";

void glat::Prismoid::draw() {
	m_program->release();
	m_program->use();
	m_vao->drawArrays(GL_LINE_STRIP_ADJACENCY, 0, m_numVert);
	m_program->release();
}

void glat::Prismoid::setPosition(const std::vector<glm::vec3>& linestrip, glm::mat4 modelViewProjection /* = glm::mat4()*/ ) {
	assert(linestrip.size() > 1);
	std::vector<glm::vec3> vertexArray;

	// we double input anchor and destination to be able to use GL_LINE_STRIP_ADJACENCY but keep it generalized to prismoids with generic chamfers
	vertexArray.push_back(linestrip.front() + glm::normalize(linestrip.front() - linestrip.at(1)));
	vertexArray.insert(vertexArray.begin() + 1, linestrip.begin(), linestrip.end());
	vertexArray.push_back(vertexArray.back() + glm::normalize(linestrip.back() - linestrip.at(linestrip.size() - 2)));

	m_numVert = vertexArray.size();
	m_positions->setData(vertexArray, GL_STATIC_DRAW);
	m_vao->binding(0)->setBuffer(m_positions, 0, sizeof(glm::vec3));

	setModelViewProjection(modelViewProjection);
}

glat::Prismoid::Prismoid() {
	setupShader(fragPrismoidShaderSource, vertPrismoidShaderSource);
	m_geomShader = glow::Shader::fromString(GL_GEOMETRY_SHADER, geomPrismoidShaderSource);
	m_program->attach(m_geomShader);

	// Position
	m_vao->binding(0)->setAttribute(0);
	m_vao->binding(0)->setFormat(3, GL_FLOAT, GL_FALSE, 0);
	m_vao->enable(0);
}

