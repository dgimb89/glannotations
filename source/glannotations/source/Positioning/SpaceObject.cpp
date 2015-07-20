#include <glannotations/Positioning/SpaceObject.h>

void glannotations::SpaceObject::setData(std::string key, globjects::ref_ptr<glannotations::AnnotationData> data) {
	m_data.insert(std::pair<std::string, globjects::ref_ptr<glannotations::AnnotationData> >(key, data));	
	setDirty(true);
}

void glannotations::SpaceObject::clearData() {
	if (m_data.size() != 0){
		m_data.clear();
		setDirty(true);
	}	
}

size_t glannotations::SpaceObject::getUID() const {
	// lazy initialization of uid
	if (m_uid == 0) {
		m_uid = generateUID();
	}
	return m_uid;
}

const glannotations::SpaceObject::AnnotationDataMap & glannotations::SpaceObject::getData() const {
	return m_data;
}

void glannotations::SpaceObject::setAnnotationClasses(std::initializer_list<std::string> annotationClasses) {
	setDirty(true);
	m_annotationClasses = annotationClasses;
}

size_t glannotations::SpaceObject::generateUID() const {
	// TODO: make this more sophisticated
	return reinterpret_cast<size_t>(this);
}

const std::vector<std::string>& glannotations::SpaceObject::getAnnotationClasses() const {
	return m_annotationClasses;
}
