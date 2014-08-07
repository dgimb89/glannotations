#pragma once

#include <glm/glm.hpp>

#include <glat/AbstractState.h>
#include <glat/glat_api.h>

namespace glat {
	class GLAT_API ViewportState : public glat::AbstractState {
	public:
		ViewportState(glm::vec2 llf, glm::vec2 urb);

		// Extends in screen space coordinates
		void setExtends(glm::vec2 llf, glm::vec2 urb);
		const glm::vec2& getLL() const;
		const glm::vec2& getUR() const;
		const glm::vec2 getLR() const;

		virtual bool isValid() const;
		virtual glat::BoundingBox getBoundingBox();

	protected:
		virtual void draw(const AbstractRenderer& renderer) override;

		virtual glow::ref_ptr<AbstractState> interpolateWith(const InternalState& mixState, float mix);
		virtual glow::ref_ptr<AbstractState> interpolateWith(const PathState& mixState, float mix);
		virtual glow::ref_ptr<AbstractState> interpolateWith(const ViewportState& mixState, float mix);

		virtual glow::ref_ptr<glat::AbstractState> clone() const;

		glm::vec2 m_ll, m_ur;
	};
}
