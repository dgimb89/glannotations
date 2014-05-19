#include <glat/Styles/ExternalColor.h>

glat::Styles::ExternalColor::ExternalColor(glm::vec4 color) {
	setColor(color);
}

void glat::Styles::ExternalColor::setColor(glm::vec4 color) {
	m_color = color;
}

glm::vec4 glat::Styles::ExternalColor::getColor() const {
	return m_color;
}
