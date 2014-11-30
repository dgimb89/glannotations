#include <glannotations/Styles/Outline.h>

glannotations::Styles::Outline::Outline(float width, glm::vec3 rgbaColor) {
	setWidth(width);
	setColor(rgbaColor);
}

void glannotations::Styles::Outline::setWidth(float width) {
	m_width = width;
}

void glannotations::Styles::Outline::setColor(glm::vec3 rgbaColor) {
	m_color = rgbaColor;
}

float glannotations::Styles::Outline::getWidth() const {
	return m_width;
}

glm::vec3 glannotations::Styles::Outline::getColor() const {
	return m_color;
}

