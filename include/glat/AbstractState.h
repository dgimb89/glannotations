#ifndef GLAT_ABSTRACT_STATE_H
#define GLAT_ABSTRACT_STATE_H

#include <glat/Object.h>
#include <glow/ref_ptr.h>
#include <glat/Styling.h>
#include <map>
#include <string>

namespace glat {

	class AbstractRenderer;

	typedef std::map<std::string, glow::ref_ptr<glat::Styling>> StylingList;

	namespace State {
		enum PositionAnchor { NO_ANCHOR, TOP_LEFT, TOP_RIGHT, BOTTOM_LEFT, BOTTOM_RIGHT };
		enum AutoExtend{ OFF, WIDTH, HEIGHT };
	}

	class AbstractState : public glat::Object {
		friend class NVPRFontRenderer;
	public:
		void setStyling(glat::Styling* style);

		// returns nullptr if style does not exist
		glow::ref_ptr<glat::Styling> getStyling(std::string ID);
		virtual bool isValid() = 0;
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