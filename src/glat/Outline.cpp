#include <glat/Outline.h>

glat::Style::Outline::Outline(float width, glm::vec3 rgbaColor) {
	setWidth(width);
	setColor(rgbaColor);
}

void glat::Style::Outline::setWidth(float width) {
	setDirty(true);
	m_width = width;
}

void glat::Style::Outline::setColor(glm::vec3 rgbaColor) {
	setDirty(true);
	m_color = rgbaColor;
}

float glat::Style::Outline::getWidth() {
	return m_width;
}

glm::vec3 glat::Style::Outline::getColor() {
	return m_color;
}

