#pragma once

#include <vector>
#include <memory>
#include <glowbase/ref_ptr.h>
#include <glow/Buffer.h>
#include <glowutils/Camera.h>

#include "globals.h"
#include <glat/glat_api.h>
#include <glat/DirtyFlagObject.h>
#include <glat/AbstractRenderer.h>
#include <glat/RendererFactory.h>
#include <glat/AbstractState.h>

namespace glat {
	// forward declaration
	class InternalState;
	class PathState;
	class ViewportState;

	class GLAT_API AbstractAnnotation : public glat::DirtyFlagObject {
	friend class AnnotationGroup;
	public:
		void draw();
		void setState(const glow::ref_ptr<glat::AbstractState>& state);
		glow::ref_ptr<glat::AbstractState> getState() const;
		glow::ref_ptr<glat::AbstractState> getRenderState() const;

		virtual void interpolateState(const InternalState& mixState, float mix);
		virtual void interpolateState(const PathState& mixState, float mix);
		virtual void interpolateState(const ViewportState& mixState, float mix);

		void resetInterpolation();

	protected:
		const glow::ref_ptr<glat::AbstractRenderer>& getRenderer();
		AbstractAnnotation(const glow::ref_ptr<glat::AbstractState>& state);
		void setupRenderState() const;
		glow::ref_ptr<glat::AbstractRenderer> m_renderer;
		glow::ref_ptr<glat::AbstractState> m_state;
		mutable glow::ref_ptr<glat::AbstractState> m_renderState;
	};
}