#include <glat/PNGAnnotation.h>

glat::PNGAnnotation::PNGAnnotation(const glow::ref_ptr<glat::AbstractState>& initialState, std::string fileName, const glat::RendererFactory& factory /*= RendererFactory()*/) 
	: glat::AbstractAnnotation(initialState) {
	m_fileName = fileName;
	m_renderer = factory.createRenderer(*this);
}

void glat::PNGAnnotation::setPNGFile(std::string filename, bool isDistanceField /*= false*/) {
	setDirty(true);
	m_fileName = filename;
	m_distanceField = isDistanceField;
}

void glat::PNGAnnotation::setAsDistanceField(glm::vec4 renderColor) {
	setDirty(true);
	m_distanceField = true;
	m_color = renderColor;
	// Todo: update / new renderer
}

void glat::PNGAnnotation::setAsNoDistanceField() {
	setDirty(true);
	m_distanceField = false;
	// Todo: update / new renderer
}

glm::vec4 glat::PNGAnnotation::getColor() const {
	return m_color;
}

bool glat::PNGAnnotation::isDistanceField() const {
	return m_distanceField;
}

const std::string& glat::PNGAnnotation::getFileName() const {
	return m_fileName;
}
