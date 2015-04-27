#define _USE_MATH_DEFINES
#define __STDC_LIMIT_MACROS

#include <unordered_map>
#include <cmath>
#include <mutex>

#include <glm/gtc/type_ptr.hpp>
#include <glbinding/gl/enum.h>
#include <globjects/globjects.h>
#include <globjects/Buffer.h>

#include <glannotations/globals.h>


// important! make sure that glbinding global objects are created BEFORE the matrizesBufferMap on global space
struct MatrizesBuffer {
	globjects::ref_ptr<globjects::Buffer> buffer;
	glm::mat4 view = glm::mat4(1);
	glm::mat4 projection = glm::mat4(1);
	float aspectRatio;
};

struct MatrizesBufferMap : public std::unordered_map < gl::GLuint, MatrizesBuffer > {
	MatrizesBufferMap() {
		globjects::init();
	}
};

std::mutex* getMutex() {
	static std::mutex* mutex = new std::mutex;
	return mutex;
}

MatrizesBufferMap* getMatricesBufferMap() {
	static MatrizesBufferMap* matricesBufferMap = new MatrizesBufferMap;
	return matricesBufferMap;
}

gl::GLsizeiptr matrixBlockSize() {
	return sizeof(glm::mat4) * 2;
}

bool isMatricesUBOInitialiced(gl::GLuint bindingIndex /*= 0*/) {
	return (*getMatricesBufferMap()).count(bindingIndex) > 0;
}

void glannotations::setView(const glm::mat4& view, gl::GLuint bindingIndex /*= 0*/) {
	if (!isMatricesUBOInitialiced(bindingIndex)) return;
	(*getMatricesBufferMap())[bindingIndex].buffer->setSubData(0, sizeof(glm::mat4), glm::value_ptr(view));
	(*getMatricesBufferMap())[bindingIndex].view = glm::inverse(view);
}

void glannotations::setProjection(const glm::mat4& projection, gl::GLuint bindingIndex /*= 0*/) {
	if (!isMatricesUBOInitialiced(bindingIndex)) return;
	(*getMatricesBufferMap())[bindingIndex].buffer->setSubData(sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(projection));
	(*getMatricesBufferMap())[bindingIndex].projection = projection;
}

void glannotations::setAspectRatio(float aspectRatio, gl::GLuint bindingIndex /*= 0*/) {
	if (!isMatricesUBOInitialiced(bindingIndex)) return;
	(*getMatricesBufferMap())[bindingIndex].aspectRatio = aspectRatio;
}

void glannotations::updateMatricesFromCamera(const gloperate::Camera& camera, gl::GLuint bindingIndex /*= 0*/) {
	setView(camera.view(), bindingIndex);
	setProjection(camera.projection(), bindingIndex);
	setAspectRatio(camera.aspectRatio(), bindingIndex);
}

void glannotations::initializeMatricesUBO(gl::GLuint bindingIndex /*= 0*/) {
	// initializiation could be called multithreaded - thus locking is appropriate
	getMutex()->lock();
	if (!isMatricesUBOInitialiced(bindingIndex)) {
		(*getMatricesBufferMap())[bindingIndex].buffer = new globjects::Buffer;
		(*getMatricesBufferMap())[bindingIndex].buffer->setData(matrixBlockSize(), nullptr, gl::GL_STREAM_DRAW);
		(*getMatricesBufferMap())[bindingIndex].buffer->bindRange(gl::GL_UNIFORM_BUFFER, bindingIndex, 0, matrixBlockSize());
	}
	getMutex()->unlock();
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

float GLANNOTATIONS_API glannotations::getAspectRatio(gl::GLuint bindingIndex /*= 0*/) {
	if (!isMatricesUBOInitialiced(bindingIndex))
		throw std::runtime_error("UBO is not initialized");
	return (*getMatricesBufferMap())[bindingIndex].aspectRatio;
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

void GLANNOTATIONS_API glannotations::initialize() {
	globjects::init();
}

void GLANNOTATIONS_API glannotations::cleanUp() {
	delete getMatricesBufferMap();
}

glm::mat4 GLANNOTATIONS_API glannotations::getViewProjection(unsigned int bindingIndex /*= 0*/) {
	return glm::mat4(getProjection(bindingIndex) * getView(bindingIndex));
}
