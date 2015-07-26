#define __STDC_LIMIT_MACROS

#include <glannotations/AnnotationGroup.h>

#include <threadingzeug/parallelfor.h>
#include <atomic>

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

void glannotations::AnnotationGroup::update() {
	updateAnnotations();
	threadingzeug::parallel_for(0, static_cast<int>(m_annotations.size()), [this](int i) {
		m_annotations.at(i)->prepareDraw();
	});
}

void glannotations::AnnotationGroup::update(long long preparationInMicroseconds) {
    std::atomic<size_t> processIndex(0);
	size_t	nMax				= m_annotations.size(), 
			startProccessing	= m_startProcessingOffset, 
			iMax				= 0, 
			i					= 0, 
			localMax			= 0;

	// early exit for empty annotation group
	if (nMax == 0) return;
	updateAnnotations();

	std::chrono::steady_clock::time_point t1 = std::chrono::steady_clock::now();

	#pragma omp parallel default(shared) private(i, localMax)
	{
		localMax = 0;
		while (	std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - t1).count() < preparationInMicroseconds
			&&	(i = processIndex++) < nMax) {
			localMax = i;
			auto annotation = m_annotations.at(ringBufferPosition(startProccessing, i, nMax));
			annotation->prepareDraw();
		}
		#pragma omp critical
		{
			if (localMax > iMax) {
				iMax = localMax;
			}
		}
	}
	m_startProcessingOffset = ringBufferPosition(startProccessing, iMax, nMax);
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

void glannotations::AnnotationGroup::setAnnotationPositioner(const std::shared_ptr<const glannotations::AnnotationPositioner>& positioner, const globjects::ref_ptr<glannotations::AnnotationDescription>& description) {
	m_positioner = positioner;
	m_description = description;
}

void glannotations::AnnotationGroup::updateAnnotations() {
	if(m_positioner)
		for (auto& annotation : m_annotations)
		m_positioner->updateAnnotation(m_description, annotation);
}

void glannotations::AnnotationGroup::draw() {
	for (const auto& annotation : m_annotations) {
		annotation->draw();
	}
}
