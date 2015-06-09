#pragma once

#include <glm/glm.hpp>

#include <glannotations/States/AbstractState.h>
#include <glannotations/glannotations_api.h>

namespace glannotations {
	class GLANNOTATIONS_API ViewportState : public glannotations::AbstractState {
		friend class AbstractPrimitiveRenderer;
	public:
		ViewportState(glm::vec2 ll, glm::vec2 lr, glm::vec2 ur);

		// Extends in screen space coordinates
		void setExtends(glm::vec2 ll, glm::vec2 lr, glm::vec2 ur);
		const glm::vec2& getLL() const;
		const glm::vec2& getUR() const;
		const glm::vec2& getLR() const;

		virtual bool isValid() const;
		virtual glannotations::BoundingBox getBoundingBox();

	protected:
		/*!
		*	\brief		Sets the screen aspect ratio to correctly translate screen space extends into aspect ratio independent extends to ensure constraints.
		*				Changing the aspect ratio will invalidate the current state, thus initiate new positioning + sizing evaluation
		*/
		void setScreenAspectRatio(float val);
		/*!
		 *	\returns	The aspect ratio that is currently set for this ViewportState
		 */
		float getScreenAspectRatio() const;

		virtual void draw(const AbstractRenderer& renderer) override;

		virtual globjects::ref_ptr<AbstractState> interpolateWith(const QuadState& mixState, float mix);
		virtual globjects::ref_ptr<AbstractState> interpolateWith(const SplineState& mixState, float mix);
		virtual globjects::ref_ptr<AbstractState> interpolateWith(const ViewportState& mixState, float mix);

		virtual globjects::ref_ptr<glannotations::AbstractState> clone() const;

		virtual void updateExtends(glm::vec2 sourceExtends) override;

	private:
		glm::vec2 m_ll, m_lr, m_ur, m_origLL, m_origLR, m_origUR;
		float m_screenAspectRatio;
	};
}
