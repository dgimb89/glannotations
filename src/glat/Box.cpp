#include <glat/Box.h>
#include <glow/VertexAttributeBinding.h>
#include <array>

/// -------------------- Shader ---------------------
static const char*	vertShader = R"(
				#version 330
				uniform mat4 modelViewProjection;

				layout (location = 0) in vec3 position;
				layout (location = 1) in vec3 widthSpan;
				layout (location = 2) in vec3 heightSpan;
				layout (location = 3) in vec3 depthSpan;
				
				out BoxData {
					mat4 mvp;
				} box;

				out vec3 v_normal;
			
				void main()
				{
					box.mvp = modelViewProjection;
					gl_Position = vec4(position, 1.0);
					v_normal = vec3(0.f, 1.f, 0.f);
				}
				)";

static const char* geomShader = R"(
				#version 330

				layout(lines) in;
				layout(triangle_strip, max_vertices = 18) out;

				in BoxData {
					mat4 mvp;
				} box[1];

				out vec3 v_normal;
				out vec3 v_vertex;

				void main() {
					vec4 vertices[8];
					// front face
					vertices[0] = box[0].mvp * vec4(gl_in[0].gl_Position.x, gl_in[0].gl_Position.y, gl_in[0].gl_Position.z, 1.0);
					vertices[1] = box[0].mvp * vec4(gl_in[0].gl_Position.x, gl_in[1].gl_Position.y, gl_in[0].gl_Position.z, 1.0);
					vertices[2] = box[0].mvp * vec4(gl_in[1].gl_Position.x, gl_in[0].gl_Position.y, gl_in[0].gl_Position.z, 1.0);
					vertices[3] = box[0].mvp * vec4(gl_in[1].gl_Position.x, gl_in[1].gl_Position.y, gl_in[0].gl_Position.z, 1.0);

					// back face
					vertices[4] = box[0].mvp * vec4(gl_in[1].gl_Position.x, gl_in[0].gl_Position.y, gl_in[1].gl_Position.z, 1.0);
					vertices[5] = box[0].mvp * vec4(gl_in[1].gl_Position.x, gl_in[1].gl_Position.y, gl_in[1].gl_Position.z, 1.0);
					vertices[6] = box[0].mvp * vec4(gl_in[0].gl_Position.x, gl_in[0].gl_Position.y, gl_in[1].gl_Position.z, 1.0);
					vertices[7] = box[0].mvp * vec4(gl_in[0].gl_Position.x, gl_in[1].gl_Position.y, gl_in[1].gl_Position.z, 1.0);

					// normal
					vec3 vector3 = vertices[1].xyz - vertices[7].xyz;
					vec3 vector4 = vertices[3].xyz - vertices[7].xyz;
					vec3 normal2 = normalize(cross(vector3, vector4));

					vec3 vector5 = vertices[6].xyz - vertices[2].xyz;
					vec3 vector6 = vertices[4].xyz - vertices[2].xyz;
					vec3 normal3 = normalize(cross(vector5, vector6));

					// lateral surface
					gl_Position = vertices[0];v_normal = normalize(cross(vertices[0].xyz - vertices[1].xyz, vertices[2].xyz - vertices[1].xyz));v_vertex = gl_Position.xyz;
					EmitVertex();
					gl_Position = vertices[1];v_normal = normalize(cross(vertices[0].xyz - vertices[1].xyz, vertices[2].xyz - vertices[1].xyz));v_vertex = gl_Position.xyz;
					EmitVertex();
					gl_Position = vertices[2];v_normal = normalize(cross(vertices[0].xyz - vertices[1].xyz, vertices[2].xyz - vertices[1].xyz));v_vertex = gl_Position.xyz;
					EmitVertex();
					gl_Position = vertices[3];v_normal = normalize(cross(vertices[3].xyz - vertices[2].xyz, vertices[1].xyz - vertices[2].xyz));v_vertex = gl_Position.xyz;
					EmitVertex();
					gl_Position = vertices[4];v_normal = normalize(cross(vertices[4].xyz - vertices[2].xyz, vertices[3].xyz - vertices[2].xyz));v_vertex = gl_Position.xyz;
					EmitVertex();
					gl_Position = vertices[5];v_normal = normalize(cross(vertices[5].xyz - vertices[3].xyz, vertices[4].xyz - vertices[3].xyz));v_vertex = gl_Position.xyz;
					EmitVertex();
					gl_Position = vertices[6];v_normal = normalize(cross(vertices[6].xyz - vertices[5].xyz, vertices[4].xyz - vertices[5].xyz));v_vertex = gl_Position.xyz;
					EmitVertex();
					gl_Position = vertices[7];v_normal = normalize(cross(vertices[7].xyz - vertices[6].xyz, vertices[5].xyz - vertices[6].xyz));v_vertex = gl_Position.xyz;
					EmitVertex();
					gl_Position = vertices[0];v_normal = normalize(cross(vertices[0].xyz - vertices[7].xyz, vertices[6].xyz - vertices[7].xyz));v_vertex = gl_Position.xyz;
					EmitVertex();
					gl_Position = vertices[1];v_normal = normalize(cross(vertices[1].xyz - vertices[0].xyz, vertices[7].xyz - vertices[0].xyz));v_vertex = gl_Position.xyz;
					EmitVertex();
					EndPrimitive();

					// top surface
					gl_Position = vertices[1];v_normal = normal2;v_vertex = gl_Position.xyz;
					EmitVertex();
					gl_Position = vertices[7];v_normal = normal2;v_vertex = gl_Position.xyz;
					EmitVertex();
					gl_Position = vertices[3];v_normal = normal2;v_vertex = gl_Position.xyz;
					EmitVertex();
					gl_Position = vertices[5];v_normal = normal2;v_vertex = gl_Position.xyz;
					EmitVertex();
					EndPrimitive();

					// bottom surface
					gl_Position = vertices[6];v_normal = normal3;v_vertex = gl_Position.xyz;
					EmitVertex();
					gl_Position = vertices[0];v_normal = normal3;v_vertex = gl_Position.xyz;
					EmitVertex();
					gl_Position = vertices[4];v_normal = normal3;v_vertex = gl_Position.xyz;
					EmitVertex();
					gl_Position = vertices[2];v_normal = normal3;v_vertex = gl_Position.xyz;
				
					EmitVertex();
					EndPrimitive();
					
				}
				)";

static const char* fragShader = R"(
				#version 330
				uniform vec4 color;

				layout (location = 0) out vec4 fragColor;
				layout (location = 1) out vec4 normals;
				layout (location = 2) out vec4 geometry;

				in vec3 v_normal;
				in vec3 v_vertex;

				void main()
				{
					fragColor = color;
					normals	= vec4(v_normal, 1.f);
					geometry = vec4(v_vertex, 1.f);
				}
				)";


glat::Box::Box() : glat::AbstractDrawingPrimitive() {
	setupShader(fragShader, vertShader);
	m_geometryShader = glow::Shader::fromString(GL_GEOMETRY_SHADER, geomShader);
	m_program->attach(m_geometryShader);

	m_vao->binding(0)->setAttribute(0);
	m_vao->binding(0)->setFormat(3, GL_FLOAT, GL_FALSE, 0);
	m_vao->enable(0);
}

void glat::Box::draw() {
	m_program->release();
	m_program->use();
	m_vao->drawArrays(GL_LINES, 0, 2);
	m_program->release();
}

void glat::Box::setPosition(glm::vec3 llf, glm::vec3 urb) {
	std::array<glm::vec3, 2> vertexArray{ { llf, urb } };
	m_vao->binding(0)->setAttribute(0);
	m_vao->binding(0)->setFormat(3, GL_FLOAT, GL_FALSE, 0);
	m_vao->enable(0);
	m_positions->setData(vertexArray, GL_STATIC_DRAW);
	m_vao->binding(0)->setBuffer(m_positions, 0, sizeof(glm::vec3));
}