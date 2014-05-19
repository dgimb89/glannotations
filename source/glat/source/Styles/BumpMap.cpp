#include <glat/Styles/BumpMap.h>

glat::Styles::BumpMap::BumpMap(float intensity) {
	setIntensity(intensity);
}

void glat::Styles::BumpMap::setIntensity(float intensity) {
	m_intensity = intensity;
}

float glat::Styles::BumpMap::getIntensity() const {
	return m_intensity;
}
