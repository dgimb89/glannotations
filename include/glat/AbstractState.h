#ifndef GLAT_ABSTRACT_STATE_H
#define GLAT_ABSTRACT_STATE_H

#include <glow/Referenced.h>
#include <glow/ref_ptr.h>
#include <glat/Styling.h>
#include <vector>

namespace glat {

	class AbstractRenderer;

	typedef std::vector<glow::ref_ptr<glat::Styling>> StylingList;

	namespace State {
		enum PositionAnchor { NO_ANCHOR, TOP_LEFT, TOP_RIGHT, BOTTOM_LEFT, BOTTOM_RIGHT };
		enum AutoExtend{ OFF, WIDTH, HEIGHT };
	}

	class AbstractState : public glow::Referenced {
		friend class NVPRFontRenderer;
	public:
		void setStyling(glat::Styling* style);
		const StylingList& getStylings();
		bool isValid();
		void setAnchor(State::PositionAnchor anchor);
		void setAutoExtend(State::AutoExtend extensionBehaviour);

	protected:
		virtual void draw(const AbstractRenderer& renderer) const = 0;
		AbstractState();
		StylingList m_stylings;
		State::PositionAnchor m_anchor;
		State::AutoExtend m_autoExtend;
	};
}

#endif