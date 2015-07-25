#include <glannotations/Positioning/SpaceObject.h>

void glannotations::SpaceObject::setData(const globjects::ref_ptr<glannotations::AnnotationData>& data) {
	setDirty(true);
	m_data = data;
}

size_t glannotations::SpaceObject::getUID() const {
	// lazy initialization of uid
	if (m_uid == 0) {
		m_uid = generateUID();
	}
	return m_uid;
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

globjects::ref_ptr<glannotations::AnnotationData> glannotations::SpaceObject::getData() const {
	return m_data;
}

const std::string& glannotations::SpaceObject::getData(std::string dataKey) {
	return m_data->get(dataKey);
}
