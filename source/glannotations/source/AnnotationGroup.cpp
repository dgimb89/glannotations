#include <glannotations/AnnotationGroup.h>

void glannotations::AnnotationGroup::addAnnotation(const globjects::ref_ptr<glannotations::AbstractAnnotation>& annotation) {
	m_annotations.push_back(annotation);
}

void glannotations::AnnotationGroup::draw() const {
	for (const auto& annotation : m_annotations) {
			annotation->draw();
	}
}

void glannotations::AnnotationGroup::clear() {
	m_annotations.clear();
}

glannotations::AnnotationGroup::~AnnotationGroup() {

}

size_t glannotations::AnnotationGroup::size() const {
	return m_annotations.size();
}
