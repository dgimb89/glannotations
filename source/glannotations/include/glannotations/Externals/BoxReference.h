#pragma once

#include <glannotations/Externals/AbstractExternalReference.h>
#include <glannotations/glannotations_api.h>

namespace glannotations {
	namespace Utility {
		struct Segment;
	}

	class GLANNOTATIONS_API BoxReference : public glannotations::AbstractExternalReference {
	public:
		// overflow given as vec2 range: (left, right), (bottom, up)
		BoxReference(glm::vec2 widthOverflow, glm::vec2 heightOverflow, glm::vec3 depthSpan, bool onlyPositioning = true);

	protected:
		void setWidth(glm::vec3 widthSpan);
		void setHeight(glm::vec3 heightSpan);
		void setDepth(glm::vec3 depthSpan);

		virtual void draw() override;

		virtual void setupExternalReference(const QuadState& state) override;
		virtual void setupExternalReference(const SplineState& state) override;

		virtual void updatePositioning(QuadState& state) override;
		virtual void updatePositioning(SplineState& state) override;

	private:
		void fixFlickering();
		bool intersectionSegmentQuad(const glannotations::Utility::Segment& ray, const glm::vec3& a, const glm::vec3& b, const glm::vec3& c, const glm::vec3& d, glm::vec3& point, float& vOverflow, float& hOverflow);
		float calculateOverflow(const glm::vec3& a, const glm::vec3& b, const glm::vec3& point, float overflowLimit);
		void determineViewdependantSpans(glm::vec3& widthSpan, glm::vec3& heightSpan, bool bottom, float& vOverflow, float& hOverflow);

		glm::vec3 m_widthSpan, m_heightSpan, m_depthSpan, m_frontLLF;
		float m_halfAnnotWidth, m_halfAnnotHeight;

	};
}
