#include <glat/BumpMap.h>

glat::Style::BumpMap::BumpMap(float intensity) {
	setIntensity(intensity);
}

void glat::Style::BumpMap::setIntensity(float intensity) {
	setDirty(true);
	m_intensity = intensity;
}

float glat::Style::BumpMap::getIntensity() {
	return m_intensity;
}
