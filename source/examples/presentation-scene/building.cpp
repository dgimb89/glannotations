#include "building.h"
#include <glbinding/gl/bitfield.h>
#include "../../glat/source/ShaderSources.hpp" // dirty hack, but we just do it here
#include <globjects/VertexAttributeBinding.h>
#include <array>

/// -------------------- Shader ---------------------

static const char* geomShader = R"(
				#version 330
				### MATRIX_BLOCK ###

				layout(lines) in;
				layout(triangle_strip, max_vertices=64) out;

				in mat4 mvp[];				

				out vec3 v_normal;
				out vec3 v_vertex;

				void main() {
					mat4 viewProjection = projectionMatrix * viewMatrix;
					vec4 vertices[8];
					// front face
					vertices[0] = viewProjection * vec4(gl_in[0].gl_Position.x, gl_in[0].gl_Position.y, gl_in[0].gl_Position.z, 1.0);
					vertices[1] = viewProjection * vec4(gl_in[0].gl_Position.x, gl_in[1].gl_Position.y, gl_in[0].gl_Position.z, 1.0);
					vertices[2] = viewProjection * vec4(gl_in[1].gl_Position.x, gl_in[0].gl_Position.y, gl_in[0].gl_Position.z, 1.0);
					vertices[3] = viewProjection * vec4(gl_in[1].gl_Position.x, gl_in[1].gl_Position.y, gl_in[0].gl_Position.z, 1.0);

					// back face
					vertices[4] = viewProjection * vec4(gl_in[1].gl_Position.x, gl_in[0].gl_Position.y, gl_in[1].gl_Position.z, 1.0);
					vertices[5] = viewProjection * vec4(gl_in[1].gl_Position.x, gl_in[1].gl_Position.y, gl_in[1].gl_Position.z, 1.0);
					vertices[6] = viewProjection * vec4(gl_in[0].gl_Position.x, gl_in[0].gl_Position.y, gl_in[1].gl_Position.z, 1.0);
					vertices[7] = viewProjection * vec4(gl_in[0].gl_Position.x, gl_in[1].gl_Position.y, gl_in[1].gl_Position.z, 1.0);

					// normal
					vec3 vector3 = vertices[1].xyz - vertices[7].xyz;
					vec3 vector4 = vertices[3].xyz - vertices[7].xyz;
					vec3 normal2 = normalize(cross(vector3, vector4));

					vec3 vector5 = vertices[6].xyz - vertices[2].xyz;
					vec3 vector6 = vertices[4].xyz - vertices[2].xyz;
					vec3 normal3 = normalize(cross(vector5, vector6));

					// front face
					gl_Position = vertices[0];v_normal = normalize(cross(vertices[3].xyz - vertices[2].xyz, vertices[1].xyz - vertices[2].xyz));v_vertex = gl_Position.xyz;
					EmitVertex();
					gl_Position = vertices[1];v_normal = normalize(cross(vertices[3].xyz - vertices[2].xyz, vertices[1].xyz - vertices[2].xyz));v_vertex = gl_Position.xyz;
					EmitVertex();
					gl_Position = vertices[2];v_normal = normalize(cross(vertices[3].xyz - vertices[2].xyz, vertices[1].xyz - vertices[2].xyz));v_vertex = gl_Position.xyz;
					EmitVertex();
					EndPrimitive();
					gl_Position = vertices[1];v_normal = normalize(cross(vertices[3].xyz - vertices[2].xyz, vertices[1].xyz - vertices[2].xyz));v_vertex = gl_Position.xyz;
					EmitVertex();
					gl_Position = vertices[2];v_normal = normalize(cross(vertices[3].xyz - vertices[2].xyz, vertices[1].xyz - vertices[2].xyz));v_vertex = gl_Position.xyz;
					EmitVertex();					
					gl_Position = vertices[3];v_normal = normalize(cross(vertices[3].xyz - vertices[2].xyz, vertices[1].xyz - vertices[2].xyz));v_vertex = gl_Position.xyz;
					EmitVertex();
					EndPrimitive();

					// right face
					gl_Position = vertices[2];v_normal = normalize(cross(vertices[4].xyz - vertices[3].xyz, vertices[3].xyz - vertices[2].xyz));v_vertex = gl_Position.xyz;
					EmitVertex();					
					gl_Position = vertices[3];v_normal = normalize(cross(vertices[4].xyz - vertices[3].xyz, vertices[3].xyz - vertices[2].xyz));v_vertex = gl_Position.xyz;
					EmitVertex();
					gl_Position = vertices[4];v_normal = normalize(cross(vertices[4].xyz - vertices[3].xyz, vertices[3].xyz - vertices[2].xyz));v_vertex = gl_Position.xyz;
					EmitVertex();
					EndPrimitive();
					gl_Position = vertices[3];v_normal = normalize(cross(vertices[4].xyz - vertices[3].xyz, vertices[3].xyz - vertices[2].xyz));v_vertex = gl_Position.xyz;
					EmitVertex();
					gl_Position = vertices[4];v_normal = normalize(cross(vertices[4].xyz - vertices[3].xyz, vertices[3].xyz - vertices[2].xyz));v_vertex = gl_Position.xyz;
					EmitVertex();
					gl_Position = vertices[5];v_normal = normalize(cross(vertices[4].xyz - vertices[3].xyz, vertices[3].xyz - vertices[2].xyz));v_vertex = gl_Position.xyz;
					EmitVertex();
					EndPrimitive();

					// back face
					gl_Position = vertices[4];v_normal = normalize(cross(vertices[7].xyz - vertices[6].xyz, vertices[5].xyz - vertices[6].xyz));v_vertex = gl_Position.xyz;
					EmitVertex();
					gl_Position = vertices[5];v_normal = normalize(cross(vertices[7].xyz - vertices[6].xyz, vertices[5].xyz - vertices[6].xyz));v_vertex = gl_Position.xyz;
					EmitVertex();
					gl_Position = vertices[6];v_normal = normalize(cross(vertices[7].xyz - vertices[6].xyz, vertices[5].xyz - vertices[6].xyz));v_vertex = gl_Position.xyz;
					EmitVertex();
					EndPrimitive();
					gl_Position = vertices[5];v_normal = normalize(cross(vertices[7].xyz - vertices[6].xyz, vertices[5].xyz - vertices[6].xyz));v_vertex = gl_Position.xyz;
					EmitVertex();
					gl_Position = vertices[6];v_normal = normalize(cross(vertices[7].xyz - vertices[6].xyz, vertices[5].xyz - vertices[6].xyz));v_vertex = gl_Position.xyz;
					EmitVertex();
					gl_Position = vertices[7];v_normal = normalize(cross(vertices[7].xyz - vertices[6].xyz, vertices[5].xyz - vertices[6].xyz));v_vertex = gl_Position.xyz;
					EmitVertex();
					EndPrimitive();

					// left face
					gl_Position = vertices[6];v_normal = normalize(cross(vertices[1].xyz - vertices[0].xyz, vertices[7].xyz - vertices[0].xyz));v_vertex = gl_Position.xyz;
					EmitVertex();
					gl_Position = vertices[7];v_normal = normalize(cross(vertices[1].xyz - vertices[0].xyz, vertices[7].xyz - vertices[0].xyz));v_vertex = gl_Position.xyz;
					EmitVertex();
					gl_Position = vertices[0];v_normal = normalize(cross(vertices[1].xyz - vertices[0].xyz, vertices[7].xyz - vertices[0].xyz));v_vertex = gl_Position.xyz;
					EmitVertex();
					EndPrimitive();
					gl_Position = vertices[7];v_normal = normalize(cross(vertices[1].xyz - vertices[0].xyz, vertices[7].xyz - vertices[0].xyz));v_vertex = gl_Position.xyz;
					EmitVertex();
					gl_Position = vertices[0];v_normal = normalize(cross(vertices[1].xyz - vertices[0].xyz, vertices[7].xyz - vertices[0].xyz));v_vertex = gl_Position.xyz;
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
				//layout (location = 1) out vec4 normals;
				//layout (location = 2) out vec4 geometry;

				in vec3 v_normal;
				in vec3 v_vertex;

				const vec3 lpos = vec3(0.0, 2.0, 2.0);

				const vec3  specular = vec3(1.0, 1.0, 1.0) * 0.1;
				const float shininess = 128.0;
				const vec3  ambient = vec3(0.08, 0.10, 0.14);
				const float ambifake = 0.78;

				void main()
				{
					//fragColor = color;
					//normals	= vec4(v_normal, 1.f);
					//geometry = vec4(v_vertex, 1.f);

					vec3 n = normalize(v_normal);
					vec3 g = v_vertex;
	
					vec3 l = normalize(lpos - g);
					vec3 e = normalize(vec3(1.f, 1.f, 1.f));
					vec3 h = normalize(l + e);

					float ldotn = mix(ambifake, 1.0, dot(l, n));
					float hdotn = dot(h, n);

					vec4 phong;

					if(ldotn > 0.0)
						phong = vec4(
						  specular * clamp(pow(hdotn, shininess), 0.0, 1.0) 
						+ mix(ambient, vec3(1.0), ldotn), 1.0);
					else
						phong = vec4(ambient, 1.0);

					fragColor = color * phong;
				}
				)";


glat::Building::Building(unsigned int matricesBindingIndex) {
	setupShader(glat::ShaderSources::passThroughVS, geomShader, fragShader);
	setBindingIndex(matricesBindingIndex);
	m_vao->binding(0)->setAttribute(0);
	m_vao->binding(0)->setFormat(3, gl::GL_FLOAT, gl::GL_FALSE, 0);
	m_vao->enable(0);
}

void glat::Building::draw() {
	m_program->release();
	gl::glDisable(gl::GL_CULL_FACE);
	m_program->use();
	m_vao->drawArrays(gl::GL_LINES, 0, 2);
	m_program->release();
	gl::glEnable(gl::GL_CULL_FACE);
}

void glat::Building::setPosition(glm::vec3 llf, glm::vec3 urb) {
	std::array<glm::vec3, 2> vertexArray{ { llf, urb } };
	m_vao->binding(0)->setAttribute(0);
	m_vao->binding(0)->setFormat(3, gl::GL_FLOAT, gl::GL_FALSE, 0);
	m_vao->enable(0);
	m_positions->setData(vertexArray, gl::GL_STATIC_DRAW);
	m_vao->binding(0)->setBuffer(m_positions, 0, sizeof(glm::vec3));
}