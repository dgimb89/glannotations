#pragma once

#include <glannotations/glannotations_api.h>
#include <glannotations/AnnotationGroup.h>
#include <glannotations/Positioning/AnnotationSpace.h>
#include <glannotations/Positioning/AnnotationDescription.h>

#include <globjects/base/ref_ptr.h>

#include <initializer_list>
#include <vector>

namespace glannotations {
	class GLANNOTATIONS_API AnnotationPositioner {
	public:
		AnnotationPositioner(	const globjects::ref_ptr<AnnotationSpace> &annotationSpace, 
								const std::initializer_list<globjects::ref_ptr<AnnotationDescription> > &descriptions);
		std::vector<globjects::ref_ptr<glannotations::AnnotationGroup> > generateAnnotationGroups() const;

	private:
		globjects::ref_ptr<AnnotationSpace> m_annotationSpace;
		std::vector<globjects::ref_ptr<AnnotationDescription> > m_descriptions;
	};
}