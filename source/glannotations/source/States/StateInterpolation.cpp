#include <glannotations/States/StateInterpolation.h>

const globjects::ref_ptr<glannotations::AbstractState> glannotations::Interpolation::interpolate(const QuadState& /*first*/, const QuadState& /*second*/, float /*mix*/) {
	return nullptr;
}

const globjects::ref_ptr<glannotations::AbstractState> glannotations::Interpolation::interpolate(const QuadState& /*first*/, const SplineState& /*second*/, float /*mix*/) {
	return nullptr;
}

const globjects::ref_ptr<glannotations::AbstractState> glannotations::Interpolation::interpolate(const QuadState& /*first*/, const ViewportState& /*second*/, float /*mix*/) {
	return nullptr;
}

const globjects::ref_ptr<glannotations::AbstractState> glannotations::Interpolation::interpolate(const SplineState& /*first*/, const QuadState& /*second*/, float /*mix*/) {
	return nullptr;
}

const globjects::ref_ptr<glannotations::AbstractState> glannotations::Interpolation::interpolate(const SplineState& /*first*/, const SplineState& /*second*/, float /*mix*/) {
	return nullptr;
}

const globjects::ref_ptr<glannotations::AbstractState> glannotations::Interpolation::interpolate(const SplineState& /*first*/, const ViewportState& /*second*/, float /*mix*/) {
	return nullptr;
}

const globjects::ref_ptr<glannotations::AbstractState> glannotations::Interpolation::interpolate(const ViewportState& /*first*/, const QuadState& /*second*/, float /*mix*/) {
	return nullptr;
}

const globjects::ref_ptr<glannotations::AbstractState> glannotations::Interpolation::interpolate(const ViewportState& /*first*/, const SplineState& /*second*/, float /*mix*/) {
	return nullptr;
}

const globjects::ref_ptr<glannotations::AbstractState> glannotations::Interpolation::interpolate(const ViewportState& /*first*/, const ViewportState& /*second*/, float /*mix*/) {
	return nullptr;
}
