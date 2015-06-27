#include <glannotations/Positioning/SpaceObject.h>

void glannotations::SpaceObject::setData(std::string key, globjects::ref_ptr<glannotations::AnnotationData> data) {
	throw std::runtime_error("This function or method is not yet implemented!");
	//because none of this works:
	//m_data[key] = data;
	//m_data.insert(key, data);
	
	setDirty(true);
}

void glannotations::SpaceObject::clearData() {
	if (m_data.size() != 0){
		m_data.clear();
		setDirty(true);
	}	
}

void glannotations::SpaceObject::setUID(size_t uid) {
	if (uid != m_uid) {
		m_uid = uid;
		setDirty(true);
	}
}

size_t glannotations::SpaceObject::getUID() const {
	return m_uid;
}

const glannotations::SpaceObject::AnnotationDataMap & glannotations::SpaceObject::getData() {
	return m_data;
}
