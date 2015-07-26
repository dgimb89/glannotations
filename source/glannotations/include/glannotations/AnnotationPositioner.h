#pragma once

#include <glannotations/glannotations_api.h>
#include <glannotations/AbstractAnnotation.h>
#include <glannotations/AnnotationGroup.h>
#include <glannotations/AnnotationSpace.h>
#include <glannotations/Positioning/AnnotationDescription.h>

#include <globjects/base/ref_ptr.h>
#include <globjects/TransformFeedback.h>
#include <globjects/Program.h>
#include <globjects/Buffer.h>

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
		void updateAnnotation(const globjects::ref_ptr<AnnotationDescription>& description, globjects::ref_ptr<glannotations::AbstractAnnotation>& annotation) const;
		globjects::ref_ptr<glannotations::AbstractAnnotation> createAnnotationFor(const globjects::ref_ptr<glannotations::SpaceObject>& object, globjects::ref_ptr<AnnotationDescription>, unsigned technique) const;
		size_t evaluateMetrics(const globjects::ref_ptr<glannotations::AbstractAnnotation>& annotation) const;

	private:
		globjects::ref_ptr<AnnotationSpace> m_annotationSpace;
		globjects::ref_ptr<globjects::TransformFeedback> m_transformFeedback;
		globjects::ref_ptr<globjects::Program> m_metricsProgram;
		globjects::ref_ptr<globjects::Buffer> m_buffer;
	};
}