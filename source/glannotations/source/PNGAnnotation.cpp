#include <glannotations/PNGAnnotation.h>

glannotations::PNGAnnotation::PNGAnnotation(const globjects::ref_ptr<glannotations::AbstractState>& initialState, std::string fileName, const glannotations::RendererFactory& factory /*= RendererFactory()*/) 
	: glannotations::AbstractAnnotation(initialState) {
	m_fileName = fileName;
	m_renderer = factory.createRenderer(*this);
}

void glannotations::PNGAnnotation::setPNGFile(std::string filename, bool isDistanceField /*= false*/) {
	setDirty(true);
	m_fileName = filename;
	m_distanceField = isDistanceField;
}

void glannotations::PNGAnnotation::setAsDistanceField(glm::vec4 renderColor) {
	setDirty(true);
	m_distanceField = true;
	m_color = renderColor;
	// Todo: update / new renderer
}

void glannotations::PNGAnnotation::setAsNoDistanceField() {
	setDirty(true);
	m_distanceField = false;
	// Todo: update / new renderer
}

glm::vec4 glannotations::PNGAnnotation::getColor() const {
	return m_color;
}

bool glannotations::PNGAnnotation::isDistanceField() const {
	return m_distanceField;
}

const std::string& glannotations::PNGAnnotation::getFileName() const {
	return m_fileName;
}
