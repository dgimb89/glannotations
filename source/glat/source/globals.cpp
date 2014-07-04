#include <glm/gtc/type_ptr.hpp>
#include <glow/Buffer.h>
#include <unordered_map>

#include <glat/globals.h>
#include <iostream>

struct MatrizesBuffer {
	glow::ref_ptr < glow::Buffer > buffer;
	glm::mat4 view;
	glm::mat4 projection;
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
