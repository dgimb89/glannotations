#include <glat/FontAnnotation.h>

void glat::FontAnnotation::setText(std::string text) {
	m_text = text;
}

const std::string& glat::FontAnnotation::getText() {
	return m_text;
}

void glat::FontAnnotation::setFontName(std::string fontName) {
	// TODO: check if font is available
	setDirty(true);
	m_fontName = fontName;
}

const std::string& glat::FontAnnotation::getFontName() {
	return m_fontName;
}

void glat::FontAnnotation::setColor(glm::vec4 color) {
	m_color = color;
}

const glm::vec4 glat::FontAnnotation::getColor() {
	return m_color;
}

glat::FontAnnotation::FontAnnotation(glat::AbstractState* initialState, std::string text, glm::vec4 color, glat::RendererFactory& factory)
: FontAnnotation(initialState, factory) {
	setText(text);
	setColor(color);
}

glat::FontAnnotation::FontAnnotation(glat::AbstractState* initialState, std::string text, const glat::RendererFactory& factory)
	: FontAnnotation(initialState, factory) {
	setText(text);
	setColor(glm::vec4(0.f, 0.f, 0.f, 1.f));
}

glat::FontAnnotation::FontAnnotation(glat::AbstractState* initialState, const glat::RendererFactory& factory)
	: AbstractAnnotation(initialState) {
	m_renderer = factory.createRenderer(*this);
	setColor(glm::vec4(0.f, 0.f, 0.f, 1.f));
}