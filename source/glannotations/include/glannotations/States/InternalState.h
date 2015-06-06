#pragma once

#include <glm/glm.hpp>

#include <glannotations/States/ReferenceableState.h>
#include <glannotations/glannotations_api.h>

namespace glannotations {

	class GLANNOTATIONS_API InternalState : public glannotations::ReferenceableState {
		friend class AbstractPrimitiveRenderer;
	public:
		/*!
		 *	\brief		Creates an Annotation State which can be positioned by a Quad in world space
		 *	\param	ll	Lower left coordinate of given Quad
		 *	\param	lr	Lower right coordinate of given Quad
		 *	\param	ur	Upper right coordinate of given Quad
		 */
		InternalState(glm::vec3 ll, glm::vec3 lr, glm::vec3 ur);

		/*!
		*	\brief		Resets the extends for this State
		*	\param	ll	Lower left coordinate of given Quad
		*	\param	lr	Lower right coordinate of given Quad
		*	\param	ur	Upper right coordinate of given Quad
		*/
		void setExtends(glm::vec3 ll, glm::vec3 lr, glm::vec3 ur);

		const glm::vec3& getLL() const;
		const glm::vec3& getUR() const;
		const glm::vec3& getLR() const;

		virtual bool isValid() const override;
		virtual glannotations::BoundingBox getBoundingBox() override;

		virtual globjects::ref_ptr<glannotations::AbstractState> clone() const;
		virtual void setExternalReference(const globjects::ref_ptr<glannotations::AbstractExternalReference>& reference) override;

	protected:
		virtual void draw(const AbstractRenderer& renderer) override;

		virtual globjects::ref_ptr<AbstractState> interpolateWith(const InternalState& mixState, float mix) override;
		virtual globjects::ref_ptr<AbstractState> interpolateWith(const SplineState& mixState, float mix) override;
		virtual globjects::ref_ptr<AbstractState> interpolateWith(const ViewportState& mixState, float mix) override;

		virtual void updateExtends(glm::vec2 sourceExtends) override;

	private:
		glm::vec3 m_ll, m_ur, m_lr;
	};
}
