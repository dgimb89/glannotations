#define __STDC_LIMIT_MACROS

#include <threadingzeug/parallelfor.h>
#include <atomic>

#include <glannotations/AnnotationGroup.h>

inline size_t ringBufferPosition(const size_t& startOffset, const size_t& i, const size_t& nMax) {
	return (startOffset + i) % nMax;
}

void glannotations::AnnotationGroup::addAnnotation(const globjects::ref_ptr<glannotations::AbstractAnnotation>& annotation) {
	annotation->prepareRenderer();
	annotation->setupRenderState();
	m_mutex.lock();
	m_annotations.push_back(annotation);
	m_mutex.unlock();
}

void glannotations::AnnotationGroup::draw() const {
	threadingzeug::parallel_for(0, static_cast<int>(m_annotations.size()), [this](int i) {
		m_annotations.at(i)->prepareDraw();
	});

	for (const auto& annotation : m_annotations) {
		annotation->draw();
	}
}

void glannotations::AnnotationGroup::draw(long long preparationInMicroseconds) {
    std::atomic<size_t> processIndex(0);
	size_t	nMax				= m_annotations.size(), 
			startProccessing	= m_startProcessingOffset, 
			iMax				= 0, 
			i					= 0, 
			localMax			= 0;
	std::chrono::steady_clock::time_point t1 = std::chrono::steady_clock::now();

	#pragma omp parallel default(shared) private(i, localMax)
	{
		localMax = 0;
		while (	std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - t1).count() < preparationInMicroseconds
			&&	(i = processIndex++) < nMax) {
			localMax = i;
			m_annotations.at(ringBufferPosition(startProccessing, i, nMax))->prepareDraw();
		}
		#pragma omp critical
		{
			if (localMax > iMax) {
				iMax = localMax;
			}
		}
	}
	m_startProcessingOffset = ringBufferPosition(startProccessing, iMax, nMax);

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
