#pragma once

#include <globjects/base/ref_ptr.h>
#include <map>
#include <string>
#include <memory>
#include <glm/glm.hpp>

#include <glannotations/DirtyFlagObject.h>
#include <glannotations/Styling.h>
#include <glannotations/BoundingBox.h>
#include <glannotations/glannotations_api.h>

namespace glannotations {

	class AbstractRenderer;
	class AbstractAnnotation;
	class ViewportState;
	class InternalState;
	class PathState;

    typedef std::map<std::string, globjects::ref_ptr<glannotations::Styling> > StylingList;

	namespace State {
		enum GLANNOTATIONS_API VerticalAnchor { MIDDLE, BOTTOM, TOP, SCALE_HEIGHT};
		enum GLANNOTATIONS_API HorizontalAnchor { CENTER, LEFT, RIGHT, SCALE_WIDTH };
	}

	class GLANNOTATIONS_API AbstractState : public glannotations::DirtyFlagObject {
		friend class NVPRRenderer;
		friend class DistanceFieldFontRenderer;
		friend class DistanceFieldPNGRenderer;

	public:

		// returns nullptr if style does not exist
		const glannotations::Styling* getStyling(std::string ID) const;
		const glannotations::StylingList& getStylings() const;
		void setStyling(const globjects::ref_ptr<glannotations::Styling>& style);
		void setStylings(const StylingList& stylings);

		virtual bool isValid() const = 0;
		virtual globjects::ref_ptr<glannotations::AbstractState> clone() const = 0;
		virtual glannotations::BoundingBox getBoundingBox() = 0;

		void setMaximumLineHeight(float height);
		float getMaximumLineHeight() const;
		// maximum lineheight of 0.f means there is no limit -- use full state space
		bool hasMaximumLineHeightConstraint() const;

		glannotations::InternalState& asInternalState();
		glannotations::PathState& asPathState();
		glannotations::ViewportState& asViewportState();

		// TODO: make use of anchors
		glannotations::State::HorizontalAnchor getHorizontalAnchor() const;
		void setHorizontalAnchor(glannotations::State::HorizontalAnchor horizontalAnchor);

		// TODO: make use of anchors
		glannotations::State::VerticalAnchor getVerticalAnchor() const;
		void setVerticalAnchor(glannotations::State::VerticalAnchor verticalAnchor);

		virtual globjects::ref_ptr<AbstractState> interpolateWith(const InternalState& mixState, float mix) = 0;
		virtual globjects::ref_ptr<AbstractState> interpolateWith(const PathState& mixState, float mix) = 0;
		virtual globjects::ref_ptr<AbstractState> interpolateWith(const ViewportState& mixState, float mix) = 0;

	protected:
		template <typename T>
		bool exceedsLineHeightConstraint(T lr, T ur) const {
			return glm::distance(ur, lr) > getMaximumLineHeight();
		}

		void copyState(AbstractState& copyTo) const;
		virtual void draw(const AbstractRenderer& renderer) = 0;
		AbstractState();

	private:
		float m_maximumLineHeight = 0.f;
		StylingList m_stylings;
		State::VerticalAnchor m_verticalAnchor = State::VerticalAnchor::MIDDLE;
		State::HorizontalAnchor m_horizontalAnchor = State::HorizontalAnchor::CENTER;
	};
}
