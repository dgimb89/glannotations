#include <glat/FontAnnotation.h>

void glat::FontAnnotation::setText(std::string text) {
	m_text = text;
}

const std::string& glat::FontAnnotation::getText() const {
	return m_text;
}

void glat::FontAnnotation::setFontName(std::string fontName) {
	// TODO: check if font is available
	setDirty(true);
	m_fontName = fontName;
}

const std::string& glat::FontAnnotation::getFontName() const {
	return m_fontName;
}

void glat::FontAnnotation::setColor(glm::vec4 color) {
	m_color = color;
}

const glm::vec4 glat::FontAnnotation::getColor() const {
	return m_color;
}

glat::FontAnnotation::FontAnnotation(const glo::ref_ptr<glat::AbstractState>& initialState, std::string text, std::string fontName, const glat::RendererFactory& factory /*= RendererFactory()*/)
: AbstractAnnotation(initialState) {
	m_renderer = factory.createRenderer(*this);
	setText(text);
	setFontName(fontName);
}

void glat::FontAnnotation::setDirty(bool dirty) {
	glat::DirtyFlagObject::setDirty(dirty);
}

void glat::FontAnnotation::setFontSize(float ptSize) {
	m_fontPtSize = ptSize;
}

float glat::FontAnnotation::getFontSize() const {
	return m_fontPtSize;
}
