#define _USE_MATH_DEFINES
#include <glm/gtc/type_ptr.hpp>
#include <globjects/globjects.h>
#include <globjects/Buffer.h>
#include <unordered_map>
#include <cmath>

#include <glat/globals.h>
#include <iostream>

// important! make sure that glbinding global objects are created BEFORE the matrizesBufferMap on global space
struct MatrizesBuffer {
	glo::ref_ptr<glo::Buffer> buffer;
	glm::mat4 view = glm::mat4(1);
	glm::mat4 projection = glm::mat4(1);
	glm::ivec2 viewport;
	glm::vec2 viewFrustumVolume;
};

struct MatrizesBufferMap : public std::unordered_map < gl::GLuint, MatrizesBuffer > {
	MatrizesBufferMap() {
		if (!glo::isInitialized()) {
			glo::init();
		}
	}
};

MatrizesBufferMap* getMatricesBufferMap() {
	static MatrizesBufferMap* matricesBufferMap = new MatrizesBufferMap;
	return matricesBufferMap;
}

gl::GLsizeiptr matrixBlockSize() {
	return sizeof(glm::mat4) * 2;
}

void glat::setView(const glm::mat4& view, gl::GLuint bindingIndex /*= 0*/) {
	(*getMatricesBufferMap())[bindingIndex].buffer->setSubData(0, sizeof(glm::mat4), glm::value_ptr(view));
	(*getMatricesBufferMap())[bindingIndex].view = glm::inverse(view);
}

void glat::setProjection(const glm::mat4& projection, gl::GLuint bindingIndex /*= 0*/) {
	(*getMatricesBufferMap())[bindingIndex].buffer->setSubData(sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(projection));
	(*getMatricesBufferMap())[bindingIndex].projection = projection;
}

void glat::updateMatricesFromCamera(const gloutils::Camera& camera, gl::GLuint bindingIndex /*= 0*/) {
	setView(camera.view(), bindingIndex);
	setProjection(camera.projection(), bindingIndex);
	setViewport(camera.viewport(), bindingIndex);
	float fovx = camera.aspectRatio() * camera.fovy();
	setViewFrustumVolume(glm::vec2(		2 * camera.zNear() / std::sin(M_PI_2 - fovx) * std::sin(fovx),
										2 * camera.zNear() / std::sin(M_PI_2 - camera.fovy()) * std::sin(camera.fovy()))
	, bindingIndex);
}

void glat::initializeMatricesUBO(gl::GLuint bindingIndex /*= 0*/) {
	(*getMatricesBufferMap())[bindingIndex].buffer = new glo::Buffer;
	(*getMatricesBufferMap())[bindingIndex].buffer->setData(matrixBlockSize(), nullptr, gl::GL_STREAM_DRAW);
	(*getMatricesBufferMap())[bindingIndex].buffer->bindRange(gl::GL_UNIFORM_BUFFER, bindingIndex, 0, matrixBlockSize());
}

bool glat::isMatricesUBOInitialiced(gl::GLuint bindingIndex /*= 0*/) {
	return (*getMatricesBufferMap()).count(bindingIndex) > 0;
}

const glm::mat4& glat::getView(gl::GLuint bindingIndex /*= 0*/) {
	if (!isMatricesUBOInitialiced(bindingIndex))
		return glm::mat4(1);
	return (*getMatricesBufferMap())[bindingIndex].view;
}

const glm::mat4 GLAT_API & glat::getProjection(gl::GLuint bindingIndex /*= 0*/) {
	if (!isMatricesUBOInitialiced(bindingIndex))
		return glm::mat4(1);
	return (*getMatricesBufferMap())[bindingIndex].projection;
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
	(*getMatricesBufferMap())[bindingIndex].viewFrustumVolume = volumeInWorldSpace;
}

void GLAT_API glat::setViewport(const glm::ivec2& viewport, gl::GLuint bindingIndex /*= 0*/) {
	(*getMatricesBufferMap())[bindingIndex].viewport = viewport;
}

glm::vec2 GLAT_API glat::getViewFrustumVolume(gl::GLuint bindingIndex /*= 0*/) {
	return (*getMatricesBufferMap())[bindingIndex].viewFrustumVolume;
} 

glm::ivec2 GLAT_API glat::getViewport(gl::GLuint bindingIndex /*= 0*/) {
	return (*getMatricesBufferMap())[bindingIndex].viewport;
}

void GLAT_API glat::initialize() {
	glo::init();
}

void GLAT_API glat::cleanUp() {
	delete getMatricesBufferMap();
}

glm::mat4 GLAT_API glat::getViewProjection(unsigned int bindingIndex /*= 0*/) {
	return glm::mat4(getProjection(bindingIndex) * getView(bindingIndex));
}
