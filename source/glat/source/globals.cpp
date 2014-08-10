#define _USE_MATH_DEFINES
#include <glm/gtc/type_ptr.hpp>
#include <glow/glow.h>
#include <glow/Buffer.h>
#include <unordered_map>

#include <glat/globals.h>
#include <iostream>

struct MatrizesBuffer {
	glow::ref_ptr < glow::Buffer > buffer;
	glm::mat4 view = glm::mat4(1);
	glm::mat4 projection = glm::mat4(1);
	glm::ivec2 viewport;
	glm::vec2 viewFrustumVolume;
};

std::unordered_map<gl::GLuint, MatrizesBuffer > g_matricesBufferMap;

gl::GLsizeiptr matrixBlockSize() {
	return sizeof(glm::mat4) * 2;
}

void glat::setView(const glm::mat4& view, gl::GLuint bindingIndex /*= 0*/) {
	g_matricesBufferMap[bindingIndex].buffer->setSubData(0, sizeof(glm::mat4), glm::value_ptr(view));
	g_matricesBufferMap[bindingIndex].view = glm::inverse(view);
}

void glat::setProjection(const glm::mat4& projection, gl::GLuint bindingIndex /*= 0*/) {
	g_matricesBufferMap[bindingIndex].buffer->setSubData(sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(projection));
	g_matricesBufferMap[bindingIndex].projection = projection;
}

void glat::updateMatricesFromCamera(const glowutils::Camera& camera, gl::GLuint bindingIndex /*= 0*/) {
	setView(camera.view(), bindingIndex);
	setProjection(camera.projection(), bindingIndex);
	setViewport(camera.viewport(), bindingIndex);
	float fovx = camera.aspectRatio() * camera.fovy();
	setViewFrustumVolume(glm::vec2(		2 * camera.zNear() / std::sinf(M_PI_2 - fovx) * std::sinf(fovx),
										2 * camera.zNear() / std::sinf(M_PI_2 - camera.fovy()) * std::sinf(camera.fovy()))
	, bindingIndex);
}

void glat::initializeMatricesUBO(gl::GLuint bindingIndex /*= 0*/) {
	g_matricesBufferMap[bindingIndex].buffer = new glow::Buffer;
	g_matricesBufferMap[bindingIndex].buffer->setData(matrixBlockSize(), nullptr, gl::GL_STREAM_DRAW);
	g_matricesBufferMap[bindingIndex].buffer->bindRange(gl::GL_UNIFORM_BUFFER, bindingIndex, 0, matrixBlockSize());
}

bool glat::isMatricesUBOInitialiced(gl::GLuint bindingIndex /*= 0*/) {
	return g_matricesBufferMap.count(bindingIndex) > 0;
}

const glm::mat4& glat::getView(gl::GLuint bindingIndex /*= 0*/) {
	if (!isMatricesUBOInitialiced(bindingIndex))
		return glm::mat4(1);
	return g_matricesBufferMap[bindingIndex].view;
}

const glm::mat4 GLAT_API & glat::getProjection(gl::GLuint bindingIndex /*= 0*/) {
	if (!isMatricesUBOInitialiced(bindingIndex))
		return glm::mat4(1);
	return g_matricesBufferMap[bindingIndex].projection;
}


glm::vec3 GLAT_API glat::getRight(gl::GLuint bindingIndex /*= 0*/) {
	return glm::vec3(glat::getView(bindingIndex)[0]);
}

glm::vec3 GLAT_API glat::getUp(gl::GLuint bindingIndex /*= 0*/) {
	return glm::vec3(glat::getView(bindingIndex)[1]);
}

glm::vec3 GLAT_API glat::getLookAt(gl::GLuint bindingIndex /*= 0*/) {
	return glm::vec3(glat::getView(bindingIndex)[2]);
}

glm::vec3 GLAT_API glat::getEye(gl::GLuint bindingIndex /*= 0*/) {
	return glm::vec3(getView(bindingIndex)[3]);
}

void GLAT_API glat::setViewFrustumVolume(glm::vec2 volumeInWorldSpace, gl::GLuint bindingIndex /*= 0*/) {
	g_matricesBufferMap[bindingIndex].viewFrustumVolume = volumeInWorldSpace;
}

void GLAT_API glat::setViewport(const glm::ivec2& viewport, gl::GLuint bindingIndex /*= 0*/) {
	g_matricesBufferMap[bindingIndex].viewport = viewport;
}

glm::vec2 GLAT_API glat::getViewFrustumVolume(gl::GLuint bindingIndex /*= 0*/) {
	return g_matricesBufferMap[bindingIndex].viewFrustumVolume;
}

glm::ivec2 GLAT_API glat::getViewport(gl::GLuint bindingIndex /*= 0*/) {
	return g_matricesBufferMap[bindingIndex].viewport;
}

void GLAT_API glat::initialize() {
	glow::init();
}
