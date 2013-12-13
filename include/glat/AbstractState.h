#ifndef GLAT_ABSTRACT_STATE_H
#define GLAT_ABSTRACT_STATE_H

#include <glow/Referenced.h>
#include <glow/ref_ptr.h>
#include <glat/Styling.h>
#include <vector>

namespace glat {
	typedef std::vector<glow::ref_ptr<glat::Styling>> StylingList;

	namespace State {
		enum PositionAnchor { NO_ANCHOR, TOP_LEFT, TOP_RIGHT, BOTTOM_LEFT, BOTTOM_RIGHT };
		enum AutoExtend{ OFF, WIDTH, HEIGHT };
	}

	class AbstractState : public glow::Referenced {
	public:
		void addStyling(glat::Styling* style);
		const StylingList& getStylings();
		bool isValid();
		void setAnchor(State::PositionAnchor anchor);
		void setAutoExtend(State::AutoExtend extensionBehaviour);

	protected:
		AbstractState();
		StylingList m_stylings;
		State::PositionAnchor m_anchor;
		State::AutoExtend m_autoExtend;
	};
}

#endif