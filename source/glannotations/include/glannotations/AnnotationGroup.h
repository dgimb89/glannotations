#pragma once

#include <vector>
#include <mutex>
#include <glm/glm.hpp>
#include <globjects/base/Referenced.h>
#include <globjects/base/ref_ptr.h>

#include <glannotations/glannotations_api.h>
#include <glannotations/AbstractAnnotation.h>

template class GLANNOTATIONS_API globjects::ref_ptr<glannotations::AbstractAnnotation>;

#  pragma warning( push )
#  pragma warning( disable: 4251 )

namespace glannotations {

	class GLANNOTATIONS_API AnnotationGroup : public globjects::Referenced {
	public:
		virtual ~AnnotationGroup();
		void clear();
		void addAnnotation(const globjects::ref_ptr<glannotations::AbstractAnnotation>& annotation);
		void prepare() const;
		void draw() const;
		size_t size() const;

	private:
		std::vector< globjects::ref_ptr<glannotations::AbstractAnnotation> > m_annotations;
		std::mutex m_mutex;
	};
}
#  pragma warning( pop )