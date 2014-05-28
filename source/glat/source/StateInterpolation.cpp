#include <glat/StateInterpolation.h>

const glow::ref_ptr<glat::AbstractState> glat::Interpolation::interpolate(const InternalState& first, const InternalState& second, float mix) {
	return nullptr;
}

const glow::ref_ptr<glat::AbstractState> glat::Interpolation::interpolate(const InternalState& first, const InternalPathState& second, float mix) {
	return nullptr;
}

const glow::ref_ptr<glat::AbstractState> glat::Interpolation::interpolate(const InternalState& first, const ViewportState& second, float mix) {
	return nullptr;
}

const glow::ref_ptr<glat::AbstractState> glat::Interpolation::interpolate(const InternalPathState& first, const InternalState& second, float mix) {
	return nullptr;
}

const glow::ref_ptr<glat::AbstractState> glat::Interpolation::interpolate(const InternalPathState& first, const InternalPathState& second, float mix) {
	return nullptr;
}

const glow::ref_ptr<glat::AbstractState> glat::Interpolation::interpolate(const InternalPathState& first, const ViewportState& second, float mix) {
	return nullptr;
}

const glow::ref_ptr<glat::AbstractState> glat::Interpolation::interpolate(const ViewportState& first, const InternalState& second, float mix) {
	return nullptr;
}

const glow::ref_ptr<glat::AbstractState> glat::Interpolation::interpolate(const ViewportState& first, const InternalPathState& second, float mix) {
	return nullptr;
}

const glow::ref_ptr<glat::AbstractState> glat::Interpolation::interpolate(const ViewportState& first, const ViewportState& second, float mix) {
	return nullptr;
}
