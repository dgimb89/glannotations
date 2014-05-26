#pragma once

#include <glowbase/ref_ptr.h>
#include <vector>
#include <memory>

#include <glat/glat_api.h>
#include <glat/DirtyFlagObject.h>
#include <glat/AbstractRenderer.h>
#include <glat/RendererFactory.h>
#include <glat/AbstractState.h>

namespace glat {

	// forward declaration
	class InternalState;
	class InternalPathState;
	class ViewportState;

	class GLAT_API AbstractAnnotation : public glat::DirtyFlagObject {
	public:
		void draw();
		void setState(const glow::ref_ptr<glat::AbstractState>& state);
		glow::ref_ptr<glat::AbstractState> getState() const;
		glow::ref_ptr<glat::AbstractState> getRenderState() const;

		virtual void interpolateState(const InternalState& mixState, float mix);
		virtual void interpolateState(const InternalPathState& mixState, float mix);
		virtual void interpolateState(const ViewportState& mixState, float mix);

		void resetInterpolation();

	protected:
		AbstractAnnotation(const glow::ref_ptr<glat::AbstractState>& state);
		glow::ref_ptr<glat::AbstractRenderer> m_renderer;
		glow::ref_ptr<glat::AbstractState> m_state;
		glow::ref_ptr<glat::AbstractState> m_interpolatedState;
	};
}