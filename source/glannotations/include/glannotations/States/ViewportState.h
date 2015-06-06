#pragma once

#include <glm/glm.hpp>

#include <glannotations/AbstractState.h>
#include <glannotations/glannotations_api.h>

namespace glannotations {
	class GLANNOTATIONS_API ViewportState : public glannotations::AbstractState {
		friend class AbstractPrimitiveRenderer;
	public:
		ViewportState(glm::vec2 llf, glm::vec2 urb);

		// Extends in screen space coordinates
		void setExtends(glm::vec2 llf, glm::vec2 urb);
		const glm::vec2& getLL() const;
		const glm::vec2& getUR() const;
		const glm::vec2 getLR() const;

		virtual bool isValid() const;
		virtual glannotations::BoundingBox getBoundingBox();

	protected:
		virtual void draw(const AbstractRenderer& renderer) override;

		virtual globjects::ref_ptr<AbstractState> interpolateWith(const InternalState& mixState, float mix);
		virtual globjects::ref_ptr<AbstractState> interpolateWith(const SplineState& mixState, float mix);
		virtual globjects::ref_ptr<AbstractState> interpolateWith(const ViewportState& mixState, float mix);

		virtual globjects::ref_ptr<glannotations::AbstractState> clone() const;

		virtual void updateExtends(glm::vec2 sourceExtends) override;

	private:
		glm::vec2 m_ll, m_ur;
	};
}
