#pragma once

#include <glm/glm.hpp>

#include <glannotations/ReferenceableState.h>
#include <glannotations/glannotations_api.h>

namespace glat {

	class GLANNOTATIONS_API InternalState : public glat::ReferenceableState {
	public:
		// lower left, lower right, upper right
		InternalState(glm::vec3 ll, glm::vec3 lr, glm::vec3 ur);

		// Extends in world coordinates
		void setExtends(glm::vec3 ll, glm::vec3 lr, glm::vec3 ur);
		const glm::vec3& getLL() const;
		const glm::vec3& getUR() const;
		const glm::vec3& getLR() const;

		virtual bool isValid() const override;
		virtual glat::BoundingBox getBoundingBox() override;

		virtual globjects::ref_ptr<glat::AbstractState> clone() const;
		virtual void setExternalReference(const globjects::ref_ptr<glat::AbstractExternalReference>& reference);

	protected:
		virtual void draw(const AbstractRenderer& renderer) override;

		virtual globjects::ref_ptr<AbstractState> interpolateWith(const InternalState& mixState, float mix);
		virtual globjects::ref_ptr<AbstractState> interpolateWith(const PathState& mixState, float mix);
		virtual globjects::ref_ptr<AbstractState> interpolateWith(const ViewportState& mixState, float mix);


		glm::vec3 m_ll, m_ur, m_lr;
	};
}
