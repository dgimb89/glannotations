#include <glat/AbstractRenderer.h>

glat::AbstractRenderer::AbstractRenderer(unsigned int globalMatricesBindingIndex) : m_globalMatricesBindingIndex(globalMatricesBindingIndex) {
}

unsigned int glat::AbstractRenderer::getMatricesBindingIndex() const {
	return m_globalMatricesBindingIndex;
}
