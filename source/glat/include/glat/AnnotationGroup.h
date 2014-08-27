#pragma once

#include <list>
#include <glm/glm.hpp>
#include <globjects-base/Referenced.h>
#include <globjects-base/ref_ptr.h>
#include <globjects-utils/Camera.h>

#include <glat/glat_api.h>
#include <glat/AbstractAnnotation.h>

namespace glat {

	class GLAT_API AnnotationGroup : public glo::Referenced {
	public:
		void addAnnotation(const glo::ref_ptr<glat::AbstractAnnotation>& annotation);
		void draw();

	private:
		std::list< glo::ref_ptr<glat::AbstractAnnotation> > m_annotations;
	};
}