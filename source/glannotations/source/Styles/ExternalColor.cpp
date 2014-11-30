#include <glannotations/Styles/ExternalColor.h>

glannotations::Styles::ExternalColor::ExternalColor(glm::vec4 color) {
	setColor(color);
}

void glannotations::Styles::ExternalColor::setColor(glm::vec4 color) {
	m_color = color;
}

glm::vec4 glannotations::Styles::ExternalColor::getColor() const {
	return m_color;
}
