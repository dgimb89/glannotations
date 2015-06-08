#define __STDC_LIMIT_MACROS

#include <threadingzeug/parallelfor.h>

#include <glannotations/AnnotationGroup.h>

void glannotations::AnnotationGroup::addAnnotation(const globjects::ref_ptr<glannotations::AbstractAnnotation>& annotation) {
	annotation->prepareRenderer();
	annotation->setupRenderState();
	m_mutex.lock();
	m_annotations.push_back(annotation);
	m_mutex.unlock();
}

void glannotations::AnnotationGroup::draw() const {
	/*
	threadingzeug::parallel_for(0, static_cast<int>(m_annotations.size()), [this](int i) {
		m_annotations.at(i)->prepareDraw();
	});
	*/

	for (const auto& annotation : m_annotations) {
		annotation->prepareDraw();
		annotation->draw();
	}
}

inline size_t glannotations::AnnotationGroup::ringBufferPosition(size_t i) {
	return (m_processIndex + i) % m_annotations.size();
}

void glannotations::AnnotationGroup::draw(long long preparationInMicroseconds) {
	size_t i = 0;
	std::chrono::steady_clock::time_point t1 = std::chrono::steady_clock::now();
	for (i = 0; i < m_annotations.size(); ++i) {
		size_t index = ringBufferPosition(i);
		m_annotations.at(index)->prepareDraw();
		if (std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - t1).count() > preparationInMicroseconds) {
			break;
		}
	}
	m_processIndex = ringBufferPosition(i);

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

void glannotations::AnnotationGroup::prepareRenderer() const {
	for (auto& annotation : m_annotations) {
		annotation->prepareRenderer();
	}
}
