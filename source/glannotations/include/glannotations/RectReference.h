#pragma once

#include <glannotations/AbstractExternalReference.h>

namespace glannotations {
	class GLANNOTATIONS_API RectReference : public glannotations::AbstractExternalReference {
	public:
		RectReference();

	protected:
		virtual void setupExternalReference(const InternalState& state) override;
		virtual void setupExternalReference(const PathState& state) override;

		virtual void updatePositioning(InternalState& state) override;
		virtual void updatePositioning(PathState& state) override;
		glm::vec3 m_widthSpan, m_heightSpan, m_center, m_up;
		float m_halfAnnotWidth, m_halfAnnotHeight;
	};
}