#pragma once

#include <vector>
#include <mutex>

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

		/*!
		 *	\brief		Initializes all the annotation renderer necessary for this AnnotationGroup. If not called explicitly, renderer will be setup on first draw call.
		 */
		void prepareRenderer() const;
		/*!
		 *	\brief		Prepares & draws all annotations of this AnnotationGroup.
		 *	\returns	
		 */
		void draw() const;
		/*!
		 *	\brief		Draws all the annotations, but will interrupt annotation preparation (e.g. positioning updates/interpolation) once the preparationLimit timeslot is exceeded.
		 *				Annotations are prepared in round-robin fashion so processing of a annotation is guaranteed before a single other one is processed a second time.
		 */
		void draw(long long preparationInMicroseconds);

	private:
		size_t m_startProcessingOffset = 0;
		std::mutex m_mutex;
		std::vector< globjects::ref_ptr<glannotations::AbstractAnnotation> > m_annotations;
	};
}