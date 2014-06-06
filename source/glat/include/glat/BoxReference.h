#pragma once

#include <glat/AbstractExternalReference.h>
#include <glat/Box.h>
#include <glat/glat_api.h>

namespace glat {
	struct Segment;

	class GLAT_API BoxReference : public glat::AbstractExternalReference {
	public:
		BoxReference(glm::vec3 widthSpan, glm::vec3 heightSpan, glm::vec3 depthSpan, glowutils::Camera* camera, bool onlyPositioning = true);

		virtual void draw() override;

		virtual void setupExternalReference(const InternalState& state) override;
		virtual void setupExternalReference(const PathState& state) override;

		virtual void updatePositioning(InternalState& state) override;
		virtual void updatePositioning(PathState& state) override;
	protected:

		void setWidth(glm::vec3 widthSpan);
		void setHeight(glm::vec3 heightSpan);
		void setDepth(glm::vec3 depthSpan);

		inline bool intersectionSegmentQuad(const glat::Segment& ray, const glm::vec3& a, const glm::vec3& b, const glm::vec3& c, const glm::vec3& d, glm::vec3& point);
		inline glm::vec3 intersectLineWithSphere(glm::vec3 lineP, glm::vec3 lineV, glm::vec3 sphereM, float radius);

		glm::vec3 m_widthSpan, m_heightSpan, m_depthSpan, m_frontLLF;
		glow::ref_ptr<glat::Box> m_box;
	};
}