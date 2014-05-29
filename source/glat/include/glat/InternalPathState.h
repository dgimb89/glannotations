#pragma once

#include <glat/ReferenceableState.h>
#include <glat/AbstractExternalReference.h>
#include <glat/BSpline.h>
#include <glat/glat_api.h>

namespace glat {
	class GLAT_API InternalPathState : public glat::ReferenceableState {
	public:
		virtual bool isValid();
		virtual glow::ref_ptr<glat::AbstractState> clone();

	protected:
		virtual void draw(const AbstractRenderer& renderer) override;

		virtual glow::ref_ptr<AbstractState> interpolateWith(const InternalState& mixState, float mix);
		virtual glow::ref_ptr<AbstractState> interpolateWith(const InternalPathState& mixState, float mix);
		virtual glow::ref_ptr<AbstractState> interpolateWith(const ViewportState& mixState, float mix);

		std::shared_ptr<glat::BSpline> m_path;
		glow::ref_ptr<glat::AbstractExternalReference> m_externalReference = nullptr;
	};
}