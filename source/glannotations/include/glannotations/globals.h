#pragma once

#include <glm/glm.hpp>
#include <gloperate/Camera.h>
#include <globjects/base/ref_ptr.h>

#include <glannotations/glannotations_api.h>

namespace glat {
	void GLANNOTATIONS_API initializeMatricesUBO(unsigned int bindingIndex = 0);
	bool GLANNOTATIONS_API isMatricesUBOInitialiced(unsigned int bindingIndex = 0);

	void GLANNOTATIONS_API initialize();
	void GLANNOTATIONS_API cleanUp();

	void GLANNOTATIONS_API setView(const glm::mat4& view, unsigned int bindingIndex = 0);
	void GLANNOTATIONS_API setProjection(const glm::mat4& projection, unsigned int bindingIndex = 0);
	void GLANNOTATIONS_API setViewFrustumVolume(glm::vec2 volumeInWorldSpace, unsigned int bindingIndex = 0);
	void GLANNOTATIONS_API setViewport(const glm::ivec2& viewport, unsigned int bindingIndex = 0);

	void GLANNOTATIONS_API updateMatricesFromCamera(const gloperate::Camera& camera, unsigned int bindingIndex = 0);

	const glm::mat4 GLANNOTATIONS_API &getView(unsigned int bindingIndex = 0);
	const glm::mat4 GLANNOTATIONS_API &getProjection(unsigned int bindingIndex = 0);
	glm::mat4 GLANNOTATIONS_API getViewProjection(unsigned int bindingIndex = 0);

	glm::vec3 GLANNOTATIONS_API getRight(unsigned int bindingIndex = 0);
	glm::vec3 GLANNOTATIONS_API getUp(unsigned int bindingIndex = 0);
	glm::vec3 GLANNOTATIONS_API getLookAt(unsigned int bindingIndex = 0);
	glm::vec3 GLANNOTATIONS_API getEye(unsigned int bindingIndex = 0);
	glm::vec2 GLANNOTATIONS_API getViewFrustumVolume(unsigned int bindingIndex = 0);
	glm::ivec2 GLANNOTATIONS_API getViewport(unsigned int bindingIndex = 0);
}