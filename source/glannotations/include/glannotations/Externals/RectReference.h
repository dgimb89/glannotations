#pragma once

#include <glannotations/Externals/AbstractExternalReference.h>

namespace glannotations {
	class GLANNOTATIONS_API RectReference : public glannotations::AbstractExternalReference {
	public:
		RectReference();

	protected:
		virtual void setupExternalReference(const InternalState& state) override;
		virtual void setupExternalReference(const SplineState& state) override;

		virtual void updatePositioning(InternalState& state) override;
		virtual void updatePositioning(SplineState& state) override;
		glm::vec3 m_widthSpan, m_heightSpan, m_center, m_up;
		float m_halfAnnotWidth, m_halfAnnotHeight;
	};
}