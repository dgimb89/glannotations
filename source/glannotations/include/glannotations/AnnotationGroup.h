#pragma once

#include <vector>
#include <mutex>
#include <chrono>

#include <glm/glm.hpp>
#include <globjects/base/Referenced.h>
#include <globjects/base/ref_ptr.h>

#include <glannotations/glannotations_api.h>
#include <glannotations/AbstractAnnotation.h>

namespace glannotations {

	class GLANNOTATIONS_API AnnotationGroup : public globjects::Referenced {
	public:
		virtual ~AnnotationGroup();
		void clear();
		void addAnnotation(const globjects::ref_ptr<glannotations::AbstractAnnotation>& annotation);
		size_t size() const;

		void prepareRenderer() const;
		void draw() const;
		void draw(std::chrono::duration<double> preparationLimit);

	private:
		std::mutex m_mutex;
		std::vector< globjects::ref_ptr<glannotations::AbstractAnnotation> > m_annotations;
	};
}