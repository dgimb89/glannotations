#pragma once

#include <globjects/base/ref_ptr.h>

#include <glannotations/InternalState.h>
#include <glannotations/PathState.h>
#include <glannotations/ViewportState.h>

namespace glannotations {
	namespace Interpolation {
		const globjects::ref_ptr<glannotations::AbstractState> interpolate(const InternalState& first, const InternalState& second, float mix);
		const globjects::ref_ptr<glannotations::AbstractState> interpolate(const InternalState& first, const PathState& second, float mix);
		const globjects::ref_ptr<glannotations::AbstractState> interpolate(const InternalState& first, const ViewportState& second, float mix);

		const globjects::ref_ptr<glannotations::AbstractState> interpolate(const PathState& first, const InternalState& second, float mix);
		const globjects::ref_ptr<glannotations::AbstractState> interpolate(const PathState& first, const PathState& second, float mix);
		const globjects::ref_ptr<glannotations::AbstractState> interpolate(const PathState& first, const ViewportState& second, float mix);

		const globjects::ref_ptr<glannotations::AbstractState> interpolate(const ViewportState& first, const InternalState& second, float mix);
		const globjects::ref_ptr<glannotations::AbstractState> interpolate(const ViewportState& first, const PathState& second, float mix);
		const globjects::ref_ptr<glannotations::AbstractState> interpolate(const ViewportState& first, const ViewportState& second, float mix);
	}
}