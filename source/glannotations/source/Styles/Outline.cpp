#include <glannotations/Styles/Outline.h>

glat::Styles::Outline::Outline(float width, glm::vec3 rgbaColor) {
	setWidth(width);
	setColor(rgbaColor);
}

void glat::Styles::Outline::setWidth(float width) {
	m_width = width;
}

void glat::Styles::Outline::setColor(glm::vec3 rgbaColor) {
	m_color = rgbaColor;
}

float glat::Styles::Outline::getWidth() const {
	return m_width;
}

glm::vec3 glat::Styles::Outline::getColor() const {
	return m_color;
}

