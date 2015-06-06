#pragma once

#include <globjects/base/ref_ptr.h>
#include <map>
#include <string>
#include <memory>
#include <glm/glm.hpp>

#include <glannotations/Common/DirtyFlagObject.h>
#include <glannotations/Styles/Styling.h>
#include <glannotations/Utility/BoundingBox.h>
#include <glannotations/glannotations_api.h>

namespace glannotations {

	class AbstractRenderer;
	class AbstractAnnotation;
	class ViewportState;
	class InternalState;
	class SplineState;

    typedef std::map<std::string, globjects::ref_ptr<glannotations::Styling> > StylingList;

	namespace Anchor {
		enum GLANNOTATIONS_API VerticalAnchor { MIDDLE, BOTTOM, TOP };
		enum GLANNOTATIONS_API HorizontalAnchor { CENTER, LEFT, RIGHT };
	}

	class GLANNOTATIONS_API AbstractState : public glannotations::DirtyFlagObject {
		friend class NVPRRenderer;
		friend class DistanceFieldFontRenderer;
		friend class DistanceFieldPNGRenderer;
		friend class AbstractPrimitiveRenderer;

	public:

		// returns nullptr if style does not exist
		const glannotations::Styling* getStyling(std::string ID) const;
		const glannotations::StylingList& getStylings() const;
		void setStyling(const globjects::ref_ptr<glannotations::Styling>& style);
		void setStylings(const StylingList& stylings);

		virtual bool isValid() const = 0;
		virtual globjects::ref_ptr<glannotations::AbstractState> clone() const = 0;
		virtual glannotations::BoundingBox getBoundingBox() = 0;

		/*!
		 *	\brief		Sets a forced maximum height constraint for the annotation; Annotation extends will be cropped according to anchor settings
		 */
		void setMaximumHeight(float height);
		float getMaximumHeight() const;
		// maximum height of 0.f means there is no limit -- use full state space
		bool hasMaximumHeight() const;

		glannotations::InternalState& asInternalState();
		glannotations::SplineState& asSplineState();
		glannotations::ViewportState& asViewportState();

		/*!
		 *	\brief		Aspect ratio of the content source is enforced; Annotation Extends will be cropped according to anchor settings
		 */
		void setKeepSourceAspectRatio(bool keepAspectRatio);
		bool getSourceKeepAspectRatio() const;

		/*!
		 *	\brief		Horizontal anchoring effects the placement of the annotation on given extends (which are potentially cropped to ensure constraints)
		 *	\see		setKeepSourceAspectRatio, getMaximumHeight
		 */
		void setHorizontalAnchor(glannotations::Anchor::HorizontalAnchor horizontalAnchor);
		/*!
		*	\brief		Vertical anchoring effects the placement of the annotation on given extends (which are potentially cropped to ensure constraints)
		*	\see		setKeepSourceAspectRatio, getMaximumHeight
		*/
		void setVerticalAnchor(glannotations::Anchor::VerticalAnchor verticalAnchor);

		glannotations::Anchor::HorizontalAnchor getHorizontalAnchor() const;
		glannotations::Anchor::VerticalAnchor getVerticalAnchor() const;

		virtual globjects::ref_ptr<AbstractState> interpolateWith(const InternalState& mixState, float mix) = 0;
		virtual globjects::ref_ptr<AbstractState> interpolateWith(const SplineState& mixState, float mix) = 0;
		virtual globjects::ref_ptr<AbstractState> interpolateWith(const ViewportState& mixState, float mix) = 0;

	protected:
		virtual void updateExtends(glm::vec2 sourceExtends) = 0;
		void copyState(AbstractState& copyTo) const;
		virtual void draw(const AbstractRenderer& renderer) = 0;
		AbstractState();

		template <typename T>
		bool exceedsLineHeightConstraint(T lr, T ur) const {
			return glm::distance(ur, lr) > getMaximumHeight();
		}

		/*!
		*	\brief		If keepSourceAsspectRatio is set to true: Crop given render extends ll, lr and ur according to sourceExtends
		*/
		template<typename T>
		void cropExtends(T& ll, T& lr, T& ur, glm::vec2 sourceExtends) {
			// calculating width / height to fit in given ll,lr,ur extends and keep aspect ratio
			float height = glm::distance(lr, ur);
			float width = glm::distance(ll, lr);
			if (hasMaximumHeight() && height > getMaximumHeight()) {
				// shrink height
				height = getMaximumHeight();
			}

			// assuring aspect ratio
			if (getSourceKeepAspectRatio()) {
				float aspectRatio = sourceExtends.x / sourceExtends.y;
				if ((width / height) > aspectRatio) {
					// shrink width
					width = aspectRatio * height;
				} else {
					// shrink height
					height = width / aspectRatio;
				}
			}

			// recalculating center according to width/height + anchor settings
			T widthSpan, heightSpan;
			widthSpan = glm::normalize(lr - ll) * 0.5f * width;
			heightSpan = glm::normalize(ur - lr) * 0.5f * height;

			T center;
			switch (getHorizontalAnchor()) {
			case Anchor::LEFT:
				center = ll + widthSpan;
				break;
			case Anchor::CENTER:
				center = (ll + lr) / 2.f;
				break;
			case Anchor::RIGHT:
				center = lr - widthSpan;
				break;
			}

			switch (getVerticalAnchor()) {
			case Anchor::BOTTOM:
				center += heightSpan;
				break;
			case Anchor::MIDDLE:
				center += (ur - lr) / 2.f;
				break;
			case Anchor::TOP:
				center += (ur - lr) - heightSpan;
				break;
			}

			// settings new extends based on new center
			ll = center - heightSpan - widthSpan;
			lr = center - heightSpan + widthSpan;
			ur = center + heightSpan + widthSpan;
		}

	private:
		bool m_keepAspectRatio = false;
		float m_maximumLineHeight = 0.f;
		StylingList m_stylings;
		Anchor::VerticalAnchor m_verticalAnchor = Anchor::VerticalAnchor::MIDDLE;
		Anchor::HorizontalAnchor m_horizontalAnchor = Anchor::HorizontalAnchor::CENTER;
	};
}
