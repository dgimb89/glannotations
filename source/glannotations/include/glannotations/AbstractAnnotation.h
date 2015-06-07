#pragma once

#include <vector>
#include <memory>
#include <globjects/base/ref_ptr.h>

#include <glannotations/Common/globals.h>
#include <glannotations/glannotations_api.h>
#include <glannotations/Common/DirtyFlagObject.h>
#include <glannotations/Renderer/AbstractRenderer.h>
#include <glannotations/Renderer/RendererFactory.h>
#include <glannotations/States/AbstractState.h>

namespace glannotations {
	// forward declaration
	class InternalState;
	class SplineState;
	class ViewportState;

	class GLANNOTATIONS_API AbstractAnnotation : public glannotations::DirtyFlagObject {
	friend class AnnotationGroup;
	public:
		void prepareRenderer();
		void prepareDraw();
		void draw();
		void setState(const globjects::ref_ptr<glannotations::AbstractState>& state);
		globjects::ref_ptr<glannotations::AbstractState> getState() const;
		globjects::ref_ptr<glannotations::AbstractState> getRenderState() const;
		bool isOnNearplane() const;
		// is defered but called automatically -- for optimzation purposes call when annotation is fully set up
		void setupRenderState() const;

		// should internal interpolation be offered? - maybe just at state level, but user has to keep the both final states
		void interpolateState(const InternalState& mixState, float mix);
		void interpolateState(const SplineState& mixState, float mix);
		void interpolateState(const ViewportState& mixState, float mix);

		void resetInterpolation();

	protected:
		virtual ~AbstractAnnotation();
		const globjects::ref_ptr<glannotations::AbstractRenderer>& getRenderer();
		AbstractAnnotation(const globjects::ref_ptr<glannotations::AbstractState>& state);
		globjects::ref_ptr<glannotations::AbstractRenderer> m_renderer;

	private:
		globjects::ref_ptr<glannotations::AbstractState> m_state;
		mutable globjects::ref_ptr<glannotations::AbstractState> m_renderState;
	};
}