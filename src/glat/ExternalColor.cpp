#include <glat/ExternalColor.h>

glat::Style::ExternalColor::ExternalColor(glm::vec4 color) {
	setColor(color);
}

void glat::Style::ExternalColor::setColor(glm::vec4 color) {
	m_color = color;
}

glm::vec4 glat::Style::ExternalColor::getColor() const {
	return m_color;
}
