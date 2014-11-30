#include <glannotations/FontAnnotation.h>

void glannotations::FontAnnotation::setText(std::string text) {
	m_text = text;
}

const std::string& glannotations::FontAnnotation::getText() const {
	return m_text;
}

void glannotations::FontAnnotation::setFontName(std::string fontName) {
	// TODO: check if font is available
	setDirty(true);
	m_fontName = fontName;
}

const std::string& glannotations::FontAnnotation::getFontName() const {
	return m_fontName;
}

void glannotations::FontAnnotation::setColor(glm::vec4 color) {
	m_color = color;
}

const glm::vec4 glannotations::FontAnnotation::getColor() const {
	return m_color;
}

glannotations::FontAnnotation::FontAnnotation(const globjects::ref_ptr<glannotations::AbstractState>& initialState, std::string text, std::string fontName, const glannotations::RendererFactory& factory /*= RendererFactory()*/)
: AbstractAnnotation(initialState) {
	m_renderer = factory.createRenderer(*this);
	setText(text);
	setFontName(fontName);
}

void glannotations::FontAnnotation::setDirty(bool dirty) {
	glannotations::DirtyFlagObject::setDirty(dirty);
}

void glannotations::FontAnnotation::setFontSize(float ptSize) {
	m_fontPtSize = ptSize;
}

float glannotations::FontAnnotation::getFontSize() const {
	return m_fontPtSize;
}
