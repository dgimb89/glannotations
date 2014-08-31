#pragma once

#include <vector>
#include <memory>
#include <globjects-base/ref_ptr.h>
#include <globjects-utils/Camera.h>

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
		void setState(const glo::ref_ptr<glat::AbstractState>& state);
		glo::ref_ptr<glat::AbstractState> getState() const;
		glo::ref_ptr<glat::AbstractState> getRenderState() const;
		bool isOnNearplane() const;

		// should internal interpolation be offered? - maybe just at state level, but user has to keep the both final states
		void interpolateState(const InternalState& mixState, float mix);
		void interpolateState(const PathState& mixState, float mix);
		void interpolateState(const ViewportState& mixState, float mix);

		void resetInterpolation();

	protected:
		const glo::ref_ptr<glat::AbstractRenderer>& getRenderer();
		AbstractAnnotation(const glo::ref_ptr<glat::AbstractState>& state);
		void setupRenderState() const;
		glo::ref_ptr<glat::AbstractRenderer> m_renderer;
		glo::ref_ptr<glat::AbstractState> m_state;
		mutable glo::ref_ptr<glat::AbstractState> m_renderState;
	};
}