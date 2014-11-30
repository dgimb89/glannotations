#pragma once

#include <vector>
#include <memory>
#include <globjects/base/ref_ptr.h>

#include "globals.h"
#include <glannotations/glannotations_api.h>
#include <glannotations/DirtyFlagObject.h>
#include <glannotations/AbstractRenderer.h>
#include <glannotations/RendererFactory.h>
#include <glannotations/AbstractState.h>

namespace glannotations {
	// forward declaration
	class InternalState;
	class PathState;
	class ViewportState;

	class GLANNOTATIONS_API AbstractAnnotation : public glannotations::DirtyFlagObject {
	friend class AnnotationGroup;
	public:
		void draw();
		void setState(const globjects::ref_ptr<glannotations::AbstractState>& state);
		globjects::ref_ptr<glannotations::AbstractState> getState() const;
		globjects::ref_ptr<glannotations::AbstractState> getRenderState() const;
		bool isOnNearplane() const;

		// should internal interpolation be offered? - maybe just at state level, but user has to keep the both final states
		void interpolateState(const InternalState& mixState, float mix);
		void interpolateState(const PathState& mixState, float mix);
		void interpolateState(const ViewportState& mixState, float mix);

		void resetInterpolation();

	protected:
		const globjects::ref_ptr<glannotations::AbstractRenderer>& getRenderer();
		AbstractAnnotation(const globjects::ref_ptr<glannotations::AbstractState>& state);
		void setupRenderState() const;
		globjects::ref_ptr<glannotations::AbstractRenderer> m_renderer;
		globjects::ref_ptr<glannotations::AbstractState> m_state;
		mutable globjects::ref_ptr<glannotations::AbstractState> m_renderState;
	};
}