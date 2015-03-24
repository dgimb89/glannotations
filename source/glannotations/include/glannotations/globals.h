#pragma once

#include <glm/glm.hpp>
#include <gloperate/painter/Camera.h>
#include <globjects/base/ref_ptr.h>

#include <glannotations/glannotations_api.h>

namespace glannotations {
	void GLANNOTATIONS_API initializeMatricesUBO(unsigned int bindingIndex = 0);
	bool GLANNOTATIONS_API isMatricesUBOInitialiced(unsigned int bindingIndex = 0);

	void GLANNOTATIONS_API initialize();
	void GLANNOTATIONS_API cleanUp();

	void GLANNOTATIONS_API setView(const glm::mat4& view, unsigned int bindingIndex = 0);
	void GLANNOTATIONS_API setProjection(const glm::mat4& projection, unsigned int bindingIndex = 0);
	void GLANNOTATIONS_API setAspectRatio(float aspectRatio, unsigned int bindingIndex = 0);

	void GLANNOTATIONS_API updateMatricesFromCamera(const gloperate::Camera& camera, unsigned int bindingIndex = 0);

	const glm::mat4 GLANNOTATIONS_API &getView(unsigned int bindingIndex = 0);
	const glm::mat4 GLANNOTATIONS_API &getProjection(unsigned int bindingIndex = 0);
	float GLANNOTATIONS_API getAspectRatio(unsigned int bindingIndex = 0);
	glm::mat4 GLANNOTATIONS_API getViewProjection(unsigned int bindingIndex = 0);

	glm::vec3 GLANNOTATIONS_API getRight(unsigned int bindingIndex = 0);
	glm::vec3 GLANNOTATIONS_API getUp(unsigned int bindingIndex = 0);
	glm::vec3 GLANNOTATIONS_API getLookAt(unsigned int bindingIndex = 0);
	glm::vec3 GLANNOTATIONS_API getEye(unsigned int bindingIndex = 0);
}