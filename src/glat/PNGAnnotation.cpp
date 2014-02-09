#include <glat/PNGAnnotation.h>

glat::PNGAnnotation::PNGAnnotation(glat::AbstractState* initialState, const glat::RendererFactory& factory /*= RendererFactory()*/) : glat::AbstractAnnotation(initialState) {
	m_renderer = factory.createRenderer(*this);
}

glat::PNGAnnotation::PNGAnnotation(glat::AbstractState* initialState, std::string fileName, const glat::RendererFactory& factory /*= RendererFactory()*/) : glat::PNGAnnotation(initialState, factory) {
	m_fileName = fileName;
}

void glat::PNGAnnotation::setPNGFile(std::string filename, bool isDistanceField /*= false*/) {
	m_fileName = filename;
	m_distanceField = isDistanceField;
}

void glat::PNGAnnotation::setAsDistanceField(glm::vec4 renderColor) {
	m_distanceField = true;
	m_color = renderColor;
	// Todo: update / new renderer
}

void glat::PNGAnnotation::setAsNoDistanceField() {
	m_distanceField = false;
	// Todo: update / new renderer
}
