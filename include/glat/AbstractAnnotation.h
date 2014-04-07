#pragma once

#include <glat/DirtyFlagObject.h>
#include <glat/AbstractRenderer.h>
#include <glat/RendererFactory.h>
#include <glat/AbstractState.h>

#include <glow/ref_ptr.h>
#include <vector>
#include <memory>

namespace glat {
class AbstractAnnotation : public glat::DirtyFlagObject {
public:
	void draw();
	void setState(unsigned statePosition);
	void setInterpolatedState(unsigned firstStatePos, unsigned secondStatePos, float interpolate);
	unsigned addState(glat::AbstractState* state);
	glow::ref_ptr<glat::AbstractState> getState();

	void interpolate(const ViewportState& firstState, const ViewportState& secondState, float interpolate) const;
	void interpolate(const ViewportState& firstState, const InternalState& secondState, float interpolate) const;
	void interpolate(const InternalState& firstState, const InternalState& secondState, float interpolate) const;
	void interpolate(const ViewportState& firstState, const ExternalBoxState& secondState, float interpolate) const;
	void interpolate(const InternalState& firstState, const ExternalBoxState& secondState, float interpolate) const;
	void interpolate(const ExternalBoxState& firstState, const ExternalBoxState& secondState, float interpolate) const;

protected:
	AbstractAnnotation(glat::AbstractState* initialState);
	glow::ref_ptr<glat::AbstractRenderer> m_renderer;
	std::vector<glow::ref_ptr<glat::AbstractState> > m_states;
	mutable glow::ref_ptr<glat::AbstractState> m_activeState;
};
}