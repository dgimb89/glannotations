#include <glat/Outline.h>

glat::Style::Outline::Outline(float width, glm::vec4 rgbaColor) {
	setWidth(width);
	setColor(rgbaColor);
}

void glat::Style::Outline::setWidth(float width) {
	setDirty(true);
	m_width = width;
}

void glat::Style::Outline::setColor(glm::vec4 rgbaColor) {
	setDirty(true);
	m_color = rgbaColor;
}

