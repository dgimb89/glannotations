#include <glat/FontAnnotation.h>

void glat::FontAnnotation::setText(std::string text) {
	setDirty(true);
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

glat::FontAnnotation::FontAnnotation(std::string text) {
	setText(text);
}

glat::FontAnnotation::FontAnnotation() : AbstractAnnotation()
{

}

