#pragma once

#include <glannotations/Externals/AbstractExternalReference.h>
#include <glannotations/Renderer/Prismoid.h>
#include <glannotations/glannotations_api.h>

namespace glannotations {
	class GLANNOTATIONS_API LabelReference : public glannotations::AbstractExternalReference {
	public:
		LabelReference(glm::vec3 reference);
	protected:
		virtual void updatePositioning(InternalState& state) override;
		virtual void updatePositioning(SplineState& state) override;

		virtual void setupExternalReference(const InternalState& state) override;
		virtual void setupExternalReference(const SplineState& state) override;

		void updatePrismoid(const glm::vec3& a, const glm::vec3& b);

	private:
		globjects::ref_ptr<glannotations::Prismoid> m_prismoid;
		glm::vec3 m_annotationCenter;
		float m_width, m_height;
	};
}
