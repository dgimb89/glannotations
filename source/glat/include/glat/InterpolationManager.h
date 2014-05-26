#pragma once

#include <glowbase/ref_ptr.h>

#include <glat/InternalState.h>
#include <glat/InternalPathState.h>
#include <glat/ViewportState.h>

namespace glat {

	struct InterpolationManager {
		static const glow::ref_ptr<glat::AbstractState> interpolate(const InternalState& first, const InternalState& second, float mix);
		static const glow::ref_ptr<glat::AbstractState> interpolate(const InternalState& first, const InternalPathState& second, float mix);
		static const glow::ref_ptr<glat::AbstractState> interpolate(const InternalState& first, const ViewportState& second, float mix);

		static const glow::ref_ptr<glat::AbstractState> interpolate(const InternalPathState& first, const InternalState& second, float mix);
		static const glow::ref_ptr<glat::AbstractState> interpolate(const InternalPathState& first, const InternalPathState& second, float mix);
		static const glow::ref_ptr<glat::AbstractState> interpolate(const InternalPathState& first, const ViewportState& second, float mix);

		static const glow::ref_ptr<glat::AbstractState> interpolate(const ViewportState& first, const InternalState& second, float mix);
		static const glow::ref_ptr<glat::AbstractState> interpolate(const ViewportState& first, const InternalPathState& second, float mix);
		static const glow::ref_ptr<glat::AbstractState> interpolate(const ViewportState& first, const ViewportState& second, float mix);
	};
}