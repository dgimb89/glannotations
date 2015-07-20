#include <glannotations/Positioning/AnnotationData.h>

glannotations::AnnotationData::AnnotationData(std::string value) {
	setValue(value);
}

void glannotations::AnnotationData::setValue(std::string value) {
	setDirty(true);
	m_value = value;
}

const std::string & glannotations::AnnotationData::getValue() const {
	return m_value;
}
