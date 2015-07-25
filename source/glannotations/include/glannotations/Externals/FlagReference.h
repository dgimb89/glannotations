#pragma once

#include <glannotations/Externals/AbstractExternalReference.h>
#include <glannotations/glannotations_api.h>

namespace glannotations {
	namespace Utility {
		struct Segment;
	}

	class GLANNOTATIONS_API FlagReference : public glannotations::AbstractExternalReference {
	public:
		FlagReference(float width, glm::vec3 depthSpan, bool onlyPositioning = false);

	protected:

		virtual void draw() override;

		virtual void setupExternalReference(const QuadState& state) override;
		virtual void setupExternalReference(const SplineState& state) override;

		virtual void updatePositioning(QuadState& state) override;
		virtual void updatePositioning(SplineState& state) override;

	private:
		bool intersectionSegmentQuad(const glannotations::Utility::Segment& ray, const glm::vec3& a, const glm::vec3& b, const glm::vec3& c, const glm::vec3& d, float& hOverflow);
		float calculateOverflow(const glm::vec3& a, const glm::vec3& b, const glm::vec3& point, float overflowLimit);

		glm::vec3 m_widthSpan, m_heightSpan, m_depthSpan, m_frontLLF;
		glm::vec3 m_newLL, m_newLR, m_newUR;

		float m_width;

	};
}
