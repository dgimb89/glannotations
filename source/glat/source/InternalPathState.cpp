#include <glat/InternalPathState.h>
#include <glat/AbstractRenderer.h>
#include <glat/AbstractAnnotation.h>
#include <glat/StateInterpolation.h>


bool glat::InternalPathState::isValid() {
	throw std::logic_error("The method or operation is not implemented.");
}

void glat::InternalPathState::draw(const AbstractRenderer& renderer) {
	if (m_externalReference) {
		m_externalReference->updatePositioning(*this);
		m_externalReference->draw();
	}
	renderer.drawSetupState(*this);
}

glow::ref_ptr<glat::AbstractState> glat::InternalPathState::interpolateWith(const InternalState& mixState, float mix) {
	return glat::Interpolation::interpolate(*this, mixState, mix);
}

glow::ref_ptr<glat::AbstractState> glat::InternalPathState::interpolateWith(const InternalPathState& mixState, float mix) {
	return glat::Interpolation::interpolate(*this, mixState, mix);
}

glow::ref_ptr<glat::AbstractState> glat::InternalPathState::interpolateWith(const ViewportState& mixState, float mix) {
	return glat::Interpolation::interpolate(*this, mixState, mix);
}
