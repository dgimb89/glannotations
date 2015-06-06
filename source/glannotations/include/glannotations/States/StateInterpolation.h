#pragma once

#include <globjects/base/ref_ptr.h>

#include <glannotations/InternalState.h>
#include <glannotations/SplineState.h>
#include <glannotations/ViewportState.h>

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