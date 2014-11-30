#include <glannotations/Styles/BumpMap.h>

glannotations::Styles::BumpMap::BumpMap(float intensity) {
	setIntensity(intensity);
}

void glannotations::Styles::BumpMap::setIntensity(float intensity) {
	m_intensity = intensity;
}

float glannotations::Styles::BumpMap::getIntensity() const {
	return m_intensity;
}
