#include <glat/BumpMap.h>

glat::Style::BumpMap::BumpMap(float intensity) {
	setIntensity(intensity);
}

void glat::Style::BumpMap::setIntensity(float intensity) {
	m_intensity = intensity;
}

float glat::Style::BumpMap::getIntensity() const {
	return m_intensity;
}
