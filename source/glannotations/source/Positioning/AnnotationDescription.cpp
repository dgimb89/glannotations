#include <glannotations/Positioning/AnnotationDescription.h>

std::string glannotations::AnnotationDescription::getAnnotationClassIdentifier() {
	return m_classIdentifier;
}

void glannotations::AnnotationDescription::setAnnotationClassIdentifier(std::string annotationClass) {
	setDirty(true);
	m_classIdentifier = annotationClass;
}

glannotations::AnnotationDescription::AnnotationDescription(/*config ,*/ std::string annotationClass) {
	setDirty(true);
	setAnnotationClassIdentifier(annotationClass);

}

void glannotations::AnnotationDescription::parseConifg(/*config*/) {
	// todo: parse config
}
