#pragma once

#include <list>
#include <glm/glm.hpp>
#include <glowbase/Referenced.h>
#include <glowbase/ref_ptr.h>
#include <glowutils/Camera.h>

#include <glat/glat_api.h>
#include <glat/AbstractAnnotation.h>

namespace glat {

	class GLAT_API AnnotationGroup : public glow::Referenced {
	public:
		void addAnnotation(const glow::ref_ptr<glat::AbstractAnnotation>& annotation);
		void draw();

	private:
		std::list< glow::ref_ptr<glat::AbstractAnnotation> > m_annotations;
	};
}