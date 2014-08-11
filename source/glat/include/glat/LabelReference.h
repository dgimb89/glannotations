#pragma once

#include <glat/AbstractExternalReference.h>
#include <glat/Prismoid.h>
#include <glat/glat_api.h>

namespace glat {
	class GLAT_API LabelReference : public glat::AbstractExternalReference {
	public:
		LabelReference(glm::vec3 reference);
	protected:
		virtual void updatePositioning(InternalState& state) override;
		virtual void updatePositioning(PathState& state) override;

		virtual void setupExternalReference(const InternalState& state) override;
		virtual void setupExternalReference(const PathState& state) override;

		void updatePrismoid(const glm::vec3& a, const glm::vec3& b);

	private:
		glow::ref_ptr<glat::Prismoid> m_prismoid;
		glm::vec3 m_annotationCenter;
		float m_width, m_height;
	};
}
