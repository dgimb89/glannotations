#include <glannotations/AbstractRenderer.h>

glannotations::AbstractRenderer::AbstractRenderer(unsigned int globalMatricesBindingIndex) : m_globalMatricesBindingIndex(globalMatricesBindingIndex) {
}

unsigned int glannotations::AbstractRenderer::getMatricesBindingIndex() const {
	return m_globalMatricesBindingIndex;
}
