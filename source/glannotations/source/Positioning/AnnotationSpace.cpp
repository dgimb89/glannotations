#include <glannotations/Positioning/AnnotationSpace.h>

glannotations::AnnotationSpace::AnnotationSpace(){
	m_spaceObjects = std::unordered_map<size_t, globjects::ref_ptr<SpaceObject> >();
}

void glannotations::AnnotationSpace::addSpaceObjects(std::initializer_list<globjects::ref_ptr<SpaceObject> > spaceObjects) {
	for (auto obj : spaceObjects) {
		size_t id = retrieveUID(obj);
		m_spaceObjects.at(id) = obj;
	}	
}

void glannotations::AnnotationSpace::clear() {
	m_spaceObjects = std::unordered_map<size_t, globjects::ref_ptr<SpaceObject> >();
}

const globjects::ref_ptr<glannotations::SpaceObject> glannotations::AnnotationSpace::getSpaceObjectAtUID(size_t uid) const {
	return m_spaceObjects.at(uid);
}

const std::unordered_map<size_t, globjects::ref_ptr<glannotations::SpaceObject> > & glannotations::AnnotationSpace::getSpaceObjects() const {
	return m_spaceObjects;
}

size_t glannotations::AnnotationSpace::retrieveUID(globjects::ref_ptr<SpaceObject> obj) const {
	//not sure if "&obj" is right. I want to use the address of the objectpointer as an UID.
	//todo: calculate hash with obj AND space
	size_t uid = reinterpret_cast<size_t>(&obj);
	return uid;
}
