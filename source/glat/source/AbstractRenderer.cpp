#include <glat/AbstractRenderer.h>

glat::AbstractRenderer::AbstractRenderer(gl::GLuint globalMatricesBindingIndex) : m_globalMatricesBindingIndex(globalMatricesBindingIndex) {
}

gl::GLuint glat::AbstractRenderer::getMatricesBindingIndex() const {
	return m_globalMatricesBindingIndex;
}
