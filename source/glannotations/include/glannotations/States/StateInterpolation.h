#pragma once

#include <globjects/base/ref_ptr.h>

#include <glannotations/States/InternalState.h>
#include <glannotations/States/SplineState.h>
#include <glannotations/States/ViewportState.h>

namespace glannotations {
	namespace Interpolation {
		const globjects::ref_ptr<glannotations::AbstractState> interpolate(const InternalState& first, const InternalState& second, float mix);
		const globjects::ref_ptr<glannotations::AbstractState> interpolate(const InternalState& first, const SplineState& second, float mix);
		const globjects::ref_ptr<glannotations::AbstractState> interpolate(const InternalState& first, const ViewportState& second, float mix);

		const globjects::ref_ptr<glannotations::AbstractState> interpolate(const SplineState& first, const InternalState& second, float mix);
		const globjects::ref_ptr<glannotations::AbstractState> interpolate(const SplineState& first, const SplineState& second, float mix);
		const globjects::ref_ptr<glannotations::AbstractState> interpolate(const SplineState& first, const ViewportState& second, float mix);

		const globjects::ref_ptr<glannotations::AbstractState> interpolate(const ViewportState& first, const InternalState& second, float mix);
		const globjects::ref_ptr<glannotations::AbstractState> interpolate(const ViewportState& first, const SplineState& second, float mix);
		const globjects::ref_ptr<glannotations::AbstractState> interpolate(const ViewportState& first, const ViewportState& second, float mix);
	}
}