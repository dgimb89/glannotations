#pragma once

#include <vector>
#include <glm/glm.hpp>
#include <globjects/base/Referenced.h>
#include <globjects/base/ref_ptr.h>

#include <glannotations/glannotations_api.h>
#include <glannotations/AbstractAnnotation.h>

namespace glannotations {

	class GLANNOTATIONS_API AnnotationGroup : public globjects::Referenced {
	public:
		void clear();
		void addAnnotation(const globjects::ref_ptr<glannotations::AbstractAnnotation>& annotation);
		void draw() const;

	private:
		std::vector< globjects::ref_ptr<glannotations::AbstractAnnotation> > m_annotations;
	};
}