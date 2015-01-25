#define _USE_MATH_DEFINES
#include <glm/gtc/type_ptr.hpp>
#include <glbinding/gl/enum.h>
#include <globjects/globjects.h>
#include <globjects/Buffer.h>
#include <unordered_map>
#include <cmath>

#include <glannotations/globals.h>
#include <iostream>

// important! make sure that glbinding global objects are created BEFORE the matrizesBufferMap on global space
struct MatrizesBuffer {
	globjects::ref_ptr<globjects::Buffer> buffer;
	glm::mat4 view = glm::mat4(1);
	glm::mat4 projection = glm::mat4(1);
	glm::vec2 viewFrustumVolume;
};

struct MatrizesBufferMap : public std::unordered_map < gl::GLuint, MatrizesBuffer > {
	MatrizesBufferMap() {
		globjects::init();
	}
};

MatrizesBufferMap* getMatricesBufferMap() {
	static MatrizesBufferMap* matricesBufferMap = new MatrizesBufferMap;
	return matricesBufferMap;
}

gl::GLsizeiptr matrixBlockSize() {
	return sizeof(glm::mat4) * 2;
}

void glannotations::setView(const glm::mat4& view, gl::GLuint bindingIndex /*= 0*/) {
	(*getMatricesBufferMap())[bindingIndex].buffer->setSubData(0, sizeof(glm::mat4), glm::value_ptr(view));
	(*getMatricesBufferMap())[bindingIndex].view = glm::inverse(view);
}

void glannotations::setProjection(const glm::mat4& projection, gl::GLuint bindingIndex /*= 0*/) {
	(*getMatricesBufferMap())[bindingIndex].buffer->setSubData(sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(projection));
	(*getMatricesBufferMap())[bindingIndex].projection = projection;
}

void glannotations::updateMatricesFromCamera(const gloperate::Camera& camera, gl::GLuint bindingIndex /*= 0*/) {
	setView(camera.view(), bindingIndex);
	setProjection(camera.projection(), bindingIndex);
	float fovx = camera.aspectRatio() * camera.fovy();
	setViewFrustumVolume(glm::vec2(		2 * camera.zNear() / std::sin(M_PI_2 - fovx) * std::sin(fovx),
										2 * camera.zNear() / std::sin(M_PI_2 - camera.fovy()) * std::sin(camera.fovy()))
	, bindingIndex);
}

void glannotations::initializeMatricesUBO(gl::GLuint bindingIndex /*= 0*/) {
	(*getMatricesBufferMap())[bindingIndex].buffer = new globjects::Buffer;
	(*getMatricesBufferMap())[bindingIndex].buffer->setData(matrixBlockSize(), nullptr, gl::GL_STREAM_DRAW);
	(*getMatricesBufferMap())[bindingIndex].buffer->bindRange(gl::GL_UNIFORM_BUFFER, bindingIndex, 0, matrixBlockSize());
}

bool glannotations::isMatricesUBOInitialiced(gl::GLuint bindingIndex /*= 0*/) {
	return (*getMatricesBufferMap()).count(bindingIndex) > 0;
}

const glm::mat4& glannotations::getView(gl::GLuint bindingIndex /*= 0*/) {
	if (!isMatricesUBOInitialiced(bindingIndex))
		throw std::runtime_error("UBO is not initialized");
	return (*getMatricesBufferMap())[bindingIndex].view;
}

const glm::mat4 GLANNOTATIONS_API & glannotations::getProjection(gl::GLuint bindingIndex /*= 0*/) {
	if (!isMatricesUBOInitialiced(bindingIndex))
		throw std::runtime_error("UBO is not initialized");
	return (*getMatricesBufferMap())[bindingIndex].projection;
}


glm::vec3 GLANNOTATIONS_API glannotations::getRight(gl::GLuint bindingIndex /*= 0*/) {
	return glm::vec3(glannotations::getView(bindingIndex)[0]);
}

glm::vec3 GLANNOTATIONS_API glannotations::getUp(gl::GLuint bindingIndex /*= 0*/) {
	return glm::vec3(glannotations::getView(bindingIndex)[1]);
}

glm::vec3 GLANNOTATIONS_API glannotations::getLookAt(gl::GLuint bindingIndex /*= 0*/) {
	return glm::vec3(glannotations::getView(bindingIndex)[2]);
}

glm::vec3 GLANNOTATIONS_API glannotations::getEye(gl::GLuint bindingIndex /*= 0*/) {
	return glm::vec3(getView(bindingIndex)[3]);
}

void GLANNOTATIONS_API glannotations::setViewFrustumVolume(glm::vec2 volumeInWorldSpace, gl::GLuint bindingIndex /*= 0*/) {
	(*getMatricesBufferMap())[bindingIndex].viewFrustumVolume = volumeInWorldSpace;
}

glm::vec2 GLANNOTATIONS_API glannotations::getViewFrustumVolume(gl::GLuint bindingIndex /*= 0*/) {
	return (*getMatricesBufferMap())[bindingIndex].viewFrustumVolume;
} 

void GLANNOTATIONS_API glannotations::initialize() {
	globjects::init();
}

void GLANNOTATIONS_API glannotations::cleanUp() {
	delete getMatricesBufferMap();
}

glm::mat4 GLANNOTATIONS_API glannotations::getViewProjection(unsigned int bindingIndex /*= 0*/) {
	return glm::mat4(getProjection(bindingIndex) * getView(bindingIndex));
}
