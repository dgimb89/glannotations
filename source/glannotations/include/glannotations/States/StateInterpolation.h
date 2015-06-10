#pragma once

#include <globjects/base/ref_ptr.h>

#include <glannotations/States/QuadState.h>
#include <glannotations/States/SplineState.h>
#include <glannotations/States/ViewportState.h>

namespace glannotations {
	namespace Interpolation {
		const globjects::ref_ptr<glannotations::AbstractState> interpolate(const QuadState& first, const QuadState& second, float mix);
		const globjects::ref_ptr<glannotations::AbstractState> interpolate(const QuadState& first, const SplineState& second, float mix);
		const globjects::ref_ptr<glannotations::AbstractState> interpolate(const QuadState& first, const ViewportState& second, float mix);

		const globjects::ref_ptr<glannotations::AbstractState> interpolate(const SplineState& first, const QuadState& second, float mix);
		const globjects::ref_ptr<glannotations::AbstractState> interpolate(const SplineState& first, const SplineState& second, float mix);
		const globjects::ref_ptr<glannotations::AbstractState> interpolate(const SplineState& first, const ViewportState& second, float mix);

		const globjects::ref_ptr<glannotations::AbstractState> interpolate(const ViewportState& first, const QuadState& second, float mix);
		const globjects::ref_ptr<glannotations::AbstractState> interpolate(const ViewportState& first, const SplineState& second, float mix);
		const globjects::ref_ptr<glannotations::AbstractState> interpolate(const ViewportState& first, const ViewportState& second, float mix);
	}
}