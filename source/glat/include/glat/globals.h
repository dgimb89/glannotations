#pragma once

#include <glm/glm.hpp>
#include <glowutils/Camera.h>
#include <glowbase/ref_ptr.h>

#include <glat/glat_api.h>

namespace glat {
	void GLAT_API initializeMatricesUBO(gl::GLuint bindingIndex = 0);
	bool GLAT_API isMatricesUBOInitialiced(gl::GLuint bindingIndex = 0);

	void GLAT_API setView(const glm::mat4& view, gl::GLuint bindingIndex = 0);
	void GLAT_API setProjection(const glm::mat4& projection, gl::GLuint bindingIndex = 0)
		;
	const glm::mat4 GLAT_API &getView(gl::GLuint bindingIndex = 0);
	glm::vec3 GLAT_API getRight(gl::GLuint bindingIndex = 0);
	glm::vec3 GLAT_API getUp(gl::GLuint bindingIndex = 0);
	glm::vec3 GLAT_API getLookAt(gl::GLuint bindingIndex = 0);
	glm::vec3 GLAT_API getEye(gl::GLuint bindingIndex = 0);

	void GLAT_API updateMatricesFromCamera(const glowutils::Camera& camera, gl::GLuint bindingIndex = 0);
}