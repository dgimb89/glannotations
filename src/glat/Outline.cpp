#include <glat/Outline.h>

glat::Style::Outline::Outline(float width, glm::vec3 rgbaColor) {
	setWidth(width);
	setColor(rgbaColor);
}

void glat::Style::Outline::setWidth(float width) {
	m_width = width;
}

void glat::Style::Outline::setColor(glm::vec3 rgbaColor) {
	m_color = rgbaColor;
}

float glat::Style::Outline::getWidth() const {
	return m_width;
}

glm::vec3 glat::Style::Outline::getColor() const {
	return m_color;
}

