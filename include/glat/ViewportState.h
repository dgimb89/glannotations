#ifndef GLAT_VIEWPORT_STATE_H
#define GLAT_VIEWPORT_STATE_H

#include <glat/AbstractState.h>
#include <glm/glm.hpp>

namespace glat {

	class ViewportState : public glat::AbstractState {
	public:
		ViewportState(glm::vec2 llf, glm::vec2 urb);

		// Extends in screen space coordinates
		void setExtends(glm::vec2 llf, glm::vec2 urb);
		const glm::vec2& getLLF() const;
		const glm::vec2& getURB() const;

		virtual bool isValid();

	protected:
		virtual void draw(const AbstractRenderer& renderer) const;
		glm::vec2 m_llf, m_urb;
	};
}

#endif // GLAT_VIEWPORT_STATE_H
