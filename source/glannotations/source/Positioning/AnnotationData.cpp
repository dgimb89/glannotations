#include <glannotations/Positioning/AnnotationData.h>

void glannotations::AnnotationData::clear() {
	setDirty(true);
	m_dataMap.clear();
}

void glannotations::AnnotationData::set(std::string key, std::string value) {
	setDirty(true);
	m_dataMap[key] = value;
}

const std::string & glannotations::AnnotationData::get(std::string key) const {
	return m_dataMap.at(key);
}
