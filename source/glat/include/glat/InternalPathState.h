#pragma once

#include <glat/AbstractState.h>
#include <glat/BSpline.h>
#include <glat/glat_api.h>

namespace glat {
	class GLAT_API InternalPathState : public glat::AbstractState {
	public:
		virtual bool isValid();

		virtual void interpolate(const AbstractAnnotation& annotation, AbstractState* secondState, float interpolate) const;
		virtual void interpolate(const AbstractAnnotation& annotation, const ViewportState& viewState, float interpolate) const;
		virtual void interpolate(const AbstractAnnotation& annotation, const InternalState& internalState, float interpolate) const;
		virtual void interpolate(const AbstractAnnotation& annotation, const ExternalBoxState& externalState, float interpolate) const;

	protected:
		virtual void draw(const AbstractRenderer& renderer) const;
		std::shared_ptr<glat::BSpline> m_path;
	};
}