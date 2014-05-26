#pragma once

#include <glowbase/ref_ptr.h>
#include <map>
#include <string>
#include <memory>

#include <glat/DirtyFlagObject.h>
#include <glat/Styling.h>
#include <glat/glat_api.h>

namespace glat {

	class AbstractRenderer;
	class AbstractAnnotation;
	class ViewportState;
	class InternalState;
	class InternalPathState;
	class ExternalBoxState;
	class ExternalLabelState;

	typedef std::map<std::string, glow::ref_ptr<glat::Styling> > StylingList;

	namespace State {
		enum PositionAnchor { NO_ANCHOR, TOP_LEFT, TOP_RIGHT, BOTTOM_LEFT, BOTTOM_RIGHT };
		enum AutoExtend{ OFF, WIDTH, HEIGHT };
	}

	class GLAT_API AbstractState : public glat::DirtyFlagObject {
		friend class NVPRRenderer;
		friend class DistanceFieldFontRenderer;
		friend class DistanceFieldPNGRenderer;

	public:
		void setStyling(const glow::ref_ptr<glat::Styling>& style);

		// returns nullptr if style does not exist
		const glat::Styling* getStyling(std::string ID) const;
		virtual bool isValid() = 0;
		void setAnchor(State::PositionAnchor anchor);
		void setAutoExtend(State::AutoExtend extensionBehaviour);

		virtual glow::ref_ptr<AbstractState> interpolateWith(const InternalState& mixState, float mix) = 0;
		virtual glow::ref_ptr<AbstractState> interpolateWith(const InternalPathState& mixState, float mix) = 0;
		virtual glow::ref_ptr<AbstractState> interpolateWith(const ViewportState& mixState, float mix) = 0;

	protected:
		virtual void draw(const AbstractRenderer& renderer) = 0;
		AbstractState();
		StylingList m_stylings;
		State::PositionAnchor m_anchor;
		State::AutoExtend m_autoExtend;
	};
}