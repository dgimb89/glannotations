#pragma once

#include <glm/glm.hpp>
#include <glowutils/Camera.h>
#include <glowbase/ref_ptr.h>

#include <glat/glat_api.h>

namespace glat {
	void GLAT_API initializeMatricesUBO(unsigned int bindingIndex = 0);
	bool GLAT_API isMatricesUBOInitialiced(unsigned int bindingIndex = 0);

	void GLAT_API initialize();

	void GLAT_API setView(const glm::mat4& view, unsigned int bindingIndex = 0);
	void GLAT_API setProjection(const glm::mat4& projection, unsigned intbindingIndex = 0);
	void GLAT_API setViewFrustumVolume(glm::vec2 volumeInWorldSpace, unsigned intbindingIndex = 0);
	void GLAT_API setViewport(const glm::ivec2& viewport, unsigned intbindingIndex = 0);

	void GLAT_API updateMatricesFromCamera(const glowutils::Camera& camera, unsigned intbindingIndex = 0);

	const glm::mat4 GLAT_API &getView(unsigned intbindingIndex = 0);
	const glm::mat4 GLAT_API &getProjection(unsigned intbindingIndex = 0);
	glm::vec3 GLAT_API getRight(unsigned intbindingIndex = 0);
	glm::vec3 GLAT_API getUp(unsigned intbindingIndex = 0);
	glm::vec3 GLAT_API getLookAt(unsigned intbindingIndex = 0);
	glm::vec3 GLAT_API getEye(unsigned intbindingIndex = 0);
	glm::vec2 GLAT_API getViewFrustumVolume(unsigned intbindingIndex = 0);
	glm::ivec2 GLAT_API getViewport(unsigned intbindingIndex = 0);
}