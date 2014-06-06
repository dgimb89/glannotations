#pragma once

#include <glowbase/ref_ptr.h>

#include <glat/InternalState.h>
#include <glat/PathState.h>
#include <glat/ViewportState.h>

namespace glat {
	namespace Interpolation {
		const glow::ref_ptr<glat::AbstractState> interpolate(const InternalState& first, const InternalState& second, float mix);
		const glow::ref_ptr<glat::AbstractState> interpolate(const InternalState& first, const PathState& second, float mix);
		const glow::ref_ptr<glat::AbstractState> interpolate(const InternalState& first, const ViewportState& second, float mix);

		const glow::ref_ptr<glat::AbstractState> interpolate(const PathState& first, const InternalState& second, float mix);
		const glow::ref_ptr<glat::AbstractState> interpolate(const PathState& first, const PathState& second, float mix);
		const glow::ref_ptr<glat::AbstractState> interpolate(const PathState& first, const ViewportState& second, float mix);

		const glow::ref_ptr<glat::AbstractState> interpolate(const ViewportState& first, const InternalState& second, float mix);
		const glow::ref_ptr<glat::AbstractState> interpolate(const ViewportState& first, const PathState& second, float mix);
		const glow::ref_ptr<glat::AbstractState> interpolate(const ViewportState& first, const ViewportState& second, float mix);
	}
}