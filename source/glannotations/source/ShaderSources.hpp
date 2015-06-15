#pragma once

#include <string>

namespace glannotations {
namespace ShaderSources {

static std::string passThroughVS = R"(
	#version 330

	layout (location = 0) in vec3 position;

	void main() {
		gl_Position = vec4(position, 1.0);
	}
	)";

static std::string matrixUniformBlock = R"(
	layout (std140) uniform GlobalMatrices {
		mat4 viewMatrix;
		mat4 projectionMatrix;
	};
	)";

} // Shader
} // glat
