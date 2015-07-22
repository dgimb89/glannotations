#pragma once

#include <glannotations/glannotations_api.h>
#include <glannotations/AbstractAnnotation.h>
#include <glannotations/AnnotationGroup.h>
#include <glannotations/AnnotationSpace.h>
#include <glannotations/Positioning/AnnotationDescription.h>

#include <globjects/base/ref_ptr.h>

#include <memory>
#include <string>

namespace glannotations {
	class GLANNOTATIONS_API AnnotationPositioner : public std::enable_shared_from_this<AnnotationPositioner> {
		friend class AnnotationGroup;
	public:
		/*!
		 *	\brief		Do not delete explicitly! Uses shared_from_this
		 */
		AnnotationPositioner(const globjects::ref_ptr<AnnotationSpace> &annotationSpace);
		std::vector<globjects::ref_ptr<glannotations::AnnotationGroup> > generateAnnotationGroups(std::string configPath) const;

	protected:
		void updateAnnotation(const globjects::ref_ptr<const AnnotationDescription>& description, const globjects::ref_ptr<glannotations::AbstractAnnotation>& annotation) const;

	private:
		globjects::ref_ptr<AnnotationSpace> m_annotationSpace;
	};
}