#include <glannotations/States/StateInterpolation.h>

const globjects::ref_ptr<glannotations::AbstractState> glannotations::Interpolation::interpolate(const InternalState& first, const InternalState& second, float mix) {
	return nullptr;
}

const globjects::ref_ptr<glannotations::AbstractState> glannotations::Interpolation::interpolate(const InternalState& first, const SplineState& second, float mix) {
	return nullptr;
}

const globjects::ref_ptr<glannotations::AbstractState> glannotations::Interpolation::interpolate(const InternalState& first, const ViewportState& second, float mix) {
	return nullptr;
}

const globjects::ref_ptr<glannotations::AbstractState> glannotations::Interpolation::interpolate(const SplineState& first, const InternalState& second, float mix) {
	return nullptr;
}

const globjects::ref_ptr<glannotations::AbstractState> glannotations::Interpolation::interpolate(const SplineState& first, const SplineState& second, float mix) {
	return nullptr;
}

const globjects::ref_ptr<glannotations::AbstractState> glannotations::Interpolation::interpolate(const SplineState& first, const ViewportState& second, float mix) {
	return nullptr;
}

const globjects::ref_ptr<glannotations::AbstractState> glannotations::Interpolation::interpolate(const ViewportState& first, const InternalState& second, float mix) {
	return nullptr;
}

const globjects::ref_ptr<glannotations::AbstractState> glannotations::Interpolation::interpolate(const ViewportState& first, const SplineState& second, float mix) {
	return nullptr;
}

const globjects::ref_ptr<glannotations::AbstractState> glannotations::Interpolation::interpolate(const ViewportState& first, const ViewportState& second, float mix) {
	return nullptr;
}
