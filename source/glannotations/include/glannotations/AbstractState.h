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

namespace glat {

	class AbstractRenderer;
	class AbstractAnnotation;
	class ViewportState;
	class InternalState;
	class PathState;

	typedef GLANNOTATIONS_API std::map<std::string, globjects::ref_ptr<glat::Styling> > StylingList;

	namespace State {
		enum GLANNOTATIONS_API VerticalAnchor { MIDDLE, BOTTOM, TOP, SCALE_HEIGHT};
		enum GLANNOTATIONS_API HorizontalAnchor { CENTER, LEFT, RIGHT, SCALE_WIDTH };
	}

	class GLANNOTATIONS_API AbstractState : public glat::DirtyFlagObject {
		friend class NVPRRenderer;
		friend class DistanceFieldFontRenderer;
		friend class DistanceFieldPNGRenderer;

	public:

		// returns nullptr if style does not exist
		const glat::Styling* getStyling(std::string ID) const;
		const glat::StylingList& getStylings() const;
		void setStyling(const globjects::ref_ptr<glat::Styling>& style);
		void setStylings(const StylingList& stylings);

		virtual bool isValid() const = 0;
		virtual globjects::ref_ptr<glat::AbstractState> clone() const = 0;
		virtual glat::BoundingBox getBoundingBox() = 0;

		glat::InternalState& asInternalState();
		glat::PathState& asPathState();
		glat::ViewportState& asViewportState();

		glat::State::HorizontalAnchor getHorizontalAnchor() const;
		void setHorizontalAnchor(glat::State::HorizontalAnchor horizontalAnchor);

		glat::State::VerticalAnchor getVerticalAnchor() const;
		void setVerticalAnchor(glat::State::VerticalAnchor verticalAnchor);

		virtual globjects::ref_ptr<AbstractState> interpolateWith(const InternalState& mixState, float mix) = 0;
		virtual globjects::ref_ptr<AbstractState> interpolateWith(const PathState& mixState, float mix) = 0;
		virtual globjects::ref_ptr<AbstractState> interpolateWith(const ViewportState& mixState, float mix) = 0;

	protected:
		void setSourceDimensions(glm::ivec2 pixelDimensions, unsigned int bindingIndex);
		void setSourceDimensions(unsigned short widthPixel, unsigned short heightPixel, unsigned int bindingIndex);
		void copyState(AbstractState& copyTo) const;
		virtual void draw(const AbstractRenderer& renderer) = 0;
		AbstractState();

		glm::vec2 m_sourceExtends;

	private:
		StylingList m_stylings;
		State::VerticalAnchor m_verticalAnchor = State::VerticalAnchor::MIDDLE;
		State::HorizontalAnchor m_horizontalAnchor = State::HorizontalAnchor::CENTER;
	};
}
