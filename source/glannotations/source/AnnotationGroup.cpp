#   define __STDC_LIMIT_MACROS

#include <glannotations/AnnotationGroup.h>

void glannotations::AnnotationGroup::addAnnotation(const globjects::ref_ptr<glannotations::AbstractAnnotation>& annotation) {
	annotation->setupRenderState();
	m_mutex.lock();
	m_annotations.push_back(annotation);
	m_mutex.unlock();
}

void glannotations::AnnotationGroup::draw() const {
	for (const auto& annotation : m_annotations) {
			annotation->draw();
	}
}

void glannotations::AnnotationGroup::clear() {
	m_mutex.lock();
	m_annotations.clear();
	m_mutex.unlock();
}

glannotations::AnnotationGroup::~AnnotationGroup() {

}

size_t glannotations::AnnotationGroup::size() const {
	return m_annotations.size();
}

void glannotations::AnnotationGroup::prepare() const {
	for (auto& annotation : m_annotations) {
		annotation->prepare();
	}
}
