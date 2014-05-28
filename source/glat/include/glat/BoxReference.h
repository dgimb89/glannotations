#pragma once

#include <glat/AbstractExternalReference.h>
#include <glat/Box.h>
#include <glat/glat_api.h>

namespace glat {
	class GLAT_API BoxReference : public glat::AbstractExternalReference {
	public:
		BoxReference(glm::vec3 widthSpan, glm::vec3 heightSpan, glm::vec3 depthSpan);
		virtual void updatePositioning(InternalState& state);
		virtual void updatePositioning(InternalPathState& state);

	protected:
		glm::vec3 m_widthSpan, m_heightSpan, m_depthSpan;
	};
}