#include <glannotations/Positioning/AnnotationDescription.h>

glannotations::AnnotationDescription::AnnotationDescription(std::string annotationClass, const globjects::ref_ptr<AnnotationClassConfig>& config) {
	setAnnotationClassIdentifier(annotationClass);
	setAnnotationClassConfig(config);
}

const std::string& glannotations::AnnotationDescription::getAnnotationClassIdentifier() const {
	return m_classIdentifier;
}

void glannotations::AnnotationDescription::setAnnotationClassIdentifier(std::string annotationClass) {
	m_classIdentifier = annotationClass;
}

const globjects::ref_ptr<glannotations::AnnotationClassConfig> glannotations::AnnotationDescription::getAnnotationClassConfig() const {
	return m_config;
}

void glannotations::AnnotationDescription::setAnnotationClassConfig(const globjects::ref_ptr<AnnotationClassConfig>& config) {
	m_config = config;
}

size_t glannotations::AnnotationDescription::getNumTechniques() const {
	return m_config->techniques.size();
}
