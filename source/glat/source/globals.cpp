#include <glm/gtc/type_ptr.hpp>
#include <glow/Buffer.h>
#include <unordered_map>

#include <glat/globals.h>

std::unordered_map<gl::GLuint, glow::ref_ptr<glow::Buffer> > g_matricesBufferMap;

gl::GLsizeiptr matrixBlockSize() {
	return sizeof(glm::mat4) * 2;
}

void glat::setView(const glm::mat4& view, gl::GLuint bindingIndex /*= 0*/) {
	g_matricesBufferMap[bindingIndex]->setSubData(0, sizeof(glm::mat4), glm::value_ptr(view));
}

void glat::setProjection(const glm::mat4& projection, gl::GLuint bindingIndex /*= 0*/) {
	g_matricesBufferMap[bindingIndex]->setSubData(sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(projection));
}

void glat::updateMatricesFromCamera(const glowutils::Camera& camera, gl::GLuint bindingIndex /*= 0*/) {
	setView(camera.view(), bindingIndex);
	setProjection(camera.projection(), bindingIndex);
}

void glat::initializeMatricesUBO(gl::GLuint bindingIndex /*= 0*/) {
	g_matricesBufferMap[bindingIndex] = new glow::Buffer;
	g_matricesBufferMap[bindingIndex]->setData(matrixBlockSize(), nullptr, gl::GL_STREAM_DRAW);
	g_matricesBufferMap[bindingIndex]->bindRange(gl::GL_UNIFORM_BUFFER, bindingIndex, 0, matrixBlockSize());
}

bool GLAT_API glat::isMatricesUBOInitialiced(gl::GLuint bindingIndex /*= 0*/) {
	return g_matricesBufferMap.count(bindingIndex) > 0;
}
