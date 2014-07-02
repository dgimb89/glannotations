#pragma once

#include <glat/AbstractExternalReference.h>
#include <glat/Box.h>
#include <glat/glat_api.h>

namespace glat {
	namespace Utility {
		struct Segment;
	}

	class GLAT_API BoxReference : public glat::AbstractExternalReference {
	public:
		// overflow given as vec2 range: (left, right), (bottom, up)
		BoxReference(glm::vec2 widthOverflow, glm::vec2 heightOverflow, glm::vec3 depthSpan, bool onlyPositioning = true);

	protected:
		void setWidth(glm::vec3 widthSpan);
		void setHeight(glm::vec3 heightSpan);
		void setDepth(glm::vec3 depthSpan);

		virtual void draw() override;
		virtual void updateBindings(const glat::AbstractRenderer& renderer) override;

		virtual void setupExternalReference(const InternalState& state) override;
		virtual void setupExternalReference(const PathState& state) override;

		virtual void updatePositioning(InternalState& state) override;
		virtual void updatePositioning(PathState& state) override;

	private:
		inline void fixFlickering();
		inline bool intersectionSegmentQuad(const glat::Utility::Segment& ray, const glm::vec3& a, const glm::vec3& b, const glm::vec3& c, const glm::vec3& d, glm::vec3& point, float& vOverflow, float& hOverflow);
		inline float calculateOverflow(const glm::vec3& a, const glm::vec3& b, const glm::vec3& point, float overflowLimit);
		inline void determineViewdependantSpans(glm::vec3& widthSpan, glm::vec3& heightSpan, bool bottom, float& vOverflow, float& hOverflow);

		glm::vec3 m_widthSpan, m_heightSpan, m_depthSpan, m_frontLLF;
		float m_halfAnnotWidth, m_halfAnnotHeight;
		glow::ref_ptr<glat::Box> m_box;

	};
}