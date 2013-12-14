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

glat::FontAnnotation::FontAnnotation(glat::AbstractState* initialState, std::string text) 
	: AbstractAnnotation(initialState) {
	setText(text);
}

glat::FontAnnotation::FontAnnotation(glat::AbstractState* initialState) 
	: AbstractAnnotation(initialState) {

}

