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
	class PathState;

	typedef GLAT_API std::map<std::string, glow::ref_ptr<glat::Styling> > StylingList;

	namespace State {
		enum GLAT_API PositionAnchor { NO_ANCHOR, TOP_LEFT, TOP_RIGHT, BOTTOM_LEFT, BOTTOM_RIGHT };
		enum GLAT_API AutoExtend{ OFF, WIDTH, HEIGHT };
	}

	class GLAT_API AbstractState : public glat::DirtyFlagObject {
		friend class NVPRRenderer;
		friend class DistanceFieldFontRenderer;
		friend class DistanceFieldPNGRenderer;

	public:

		// returns nullptr if style does not exist
		const glat::Styling* getStyling(std::string ID) const;
		const glat::StylingList& getStylings() const;
		void setStyling(const glow::ref_ptr<glat::Styling>& style);
		void setStylings(const StylingList& stylings);

		virtual bool isValid() const = 0;
		virtual glow::ref_ptr<glat::AbstractState> clone() const = 0;

		glat::InternalState& asInternalState();
		glat::PathState& asPathState();
		glat::ViewportState& asViewportState();

		glat::State::PositionAnchor getAnchor() const;
		void setAnchor(State::PositionAnchor anchor);

		glat::State::AutoExtend getAutoExtend() const;
		void setAutoExtend(State::AutoExtend extensionBehaviour);

		virtual glow::ref_ptr<AbstractState> interpolateWith(const InternalState& mixState, float mix) = 0;
		virtual glow::ref_ptr<AbstractState> interpolateWith(const PathState& mixState, float mix) = 0;
		virtual glow::ref_ptr<AbstractState> interpolateWith(const ViewportState& mixState, float mix) = 0;

	protected:
		void copyState(AbstractState& copyTo) const;
		virtual void draw(const AbstractRenderer& renderer) = 0;
		AbstractState();

	private:
		StylingList m_stylings;
		State::PositionAnchor m_anchor;
		State::AutoExtend m_autoExtend;
	};
}