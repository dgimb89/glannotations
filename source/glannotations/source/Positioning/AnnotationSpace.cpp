#include <glannotations/Positioning/AnnotationSpace.h>

glannotations::AnnotationSpace::AnnotationSpace() {
	setDirty(true);
}

void glannotations::AnnotationSpace::addSpaceObjects(std::initializer_list<globjects::ref_ptr<SpaceObject> > spaceObjects) {
	setDirty(true);
	for (const auto &obj : spaceObjects) {
		for (const auto &annClass : obj->getAnnotationClasses()) {
			m_spaceObjects[annClass].push_back(obj);
		}
	}	
}

void glannotations::AnnotationSpace::clear() {
	setDirty(true);
	m_spaceObjects.clear();
}

const glannotations::AnnotationSpace::AnnotationClass_t & glannotations::AnnotationSpace::getSpaceObjectsForClass(std::string annotationClass) {
	return m_spaceObjects.at(annotationClass);
}

const glannotations::AnnotationSpace::AnnotationSpace_t & glannotations::AnnotationSpace::getSpaceObjects() const {
	return m_spaceObjects;
}

const globjects::ref_ptr<glannotations::SpaceObject> glannotations::AnnotationSpace::getSpaceObjectWithUID(size_t uid) const {
	// todo: implement
	return nullptr;
}
