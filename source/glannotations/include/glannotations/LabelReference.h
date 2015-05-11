#pragma once

#include <glannotations/AbstractExternalReference.h>
#include <glannotations/Prismoid.h>
#include <glannotations/glannotations_api.h>


template struct GLANNOTATIONS_API glm::detail::tvec3 < float, glm::precision::defaultp > ;

// disable export warnings for globjects::ref_ptr<glannotations::Prismoid> because we dont offer it in interface
#  pragma warning( push )
#  pragma warning( disable: 4251 )

namespace glannotations {
	class GLANNOTATIONS_API LabelReference : public glannotations::AbstractExternalReference {
	public:
		LabelReference(glm::vec3 reference);
	protected:
		virtual void updatePositioning(InternalState& state) override;
		virtual void updatePositioning(PathState& state) override;

		virtual void setupExternalReference(const InternalState& state) override;
		virtual void setupExternalReference(const PathState& state) override;

		void updatePrismoid(const glm::vec3& a, const glm::vec3& b);

	private:
		globjects::ref_ptr<glannotations::Prismoid> m_prismoid;
		glm::vec3 m_annotationCenter;
		float m_width, m_height;
	};
}

#  pragma warning( pop )