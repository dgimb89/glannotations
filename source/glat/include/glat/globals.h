#pragma once

#include <glm/glm.hpp>
#include <globjects-utils/Camera.h>
#include <globjects-base/ref_ptr.h>

#include <glat/glat_api.h>

namespace glat {
	void GLAT_API initializeMatricesUBO(unsigned int bindingIndex = 0);
	bool GLAT_API isMatricesUBOInitialiced(unsigned int bindingIndex = 0);

	void GLAT_API initialize();
	void GLAT_API cleanUp();

	void GLAT_API setView(const glm::mat4& view, unsigned int bindingIndex = 0);
	void GLAT_API setProjection(const glm::mat4& projection, unsigned int bindingIndex = 0);
	void GLAT_API setViewFrustumVolume(glm::vec2 volumeInWorldSpace, unsigned int bindingIndex = 0);
	void GLAT_API setViewport(const glm::ivec2& viewport, unsigned int bindingIndex = 0);

	void GLAT_API updateMatricesFromCamera(const gloutils::Camera& camera, unsigned int bindingIndex = 0);

	const glm::mat4 GLAT_API &getView(unsigned int bindingIndex = 0);
	const glm::mat4 GLAT_API &getProjection(unsigned int bindingIndex = 0);
	glm::mat4 GLAT_API getViewProjection(unsigned int bindingIndex = 0);

	glm::vec3 GLAT_API getRight(unsigned int bindingIndex = 0);
	glm::vec3 GLAT_API getUp(unsigned int bindingIndex = 0);
	glm::vec3 GLAT_API getLookAt(unsigned int bindingIndex = 0);
	glm::vec3 GLAT_API getEye(unsigned int bindingIndex = 0);
	glm::vec2 GLAT_API getViewFrustumVolume(unsigned int bindingIndex = 0);
	glm::ivec2 GLAT_API getViewport(unsigned int bindingIndex = 0);
}