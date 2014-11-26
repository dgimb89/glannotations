#include <glannotations/PathState.h>
#include <glannotations/AbstractRenderer.h>
#include <glannotations/AbstractAnnotation.h>
#include <glannotations/StateInterpolation.h>


bool glat::PathState::isValid() {
	throw std::logic_error("The method or operation is not implemented.");
}

void glat::PathState::draw(const AbstractRenderer& renderer) {
	if (m_externalReference) {
		m_externalReference->updatePositioning(*this);
		m_externalReference->draw();
	}
	renderer.drawSetupState(*this);
}

globjects::ref_ptr<glat::AbstractState> glat::PathState::interpolateWith(const InternalState& mixState, float mix) {
	return glat::Interpolation::interpolate(*this, mixState, mix);
}

globjects::ref_ptr<glat::AbstractState> glat::PathState::interpolateWith(const PathState& mixState, float mix) {
	return glat::Interpolation::interpolate(*this, mixState, mix);
}

globjects::ref_ptr<glat::AbstractState> glat::PathState::interpolateWith(const ViewportState& mixState, float mix) {
	return glat::Interpolation::interpolate(*this, mixState, mix);
}

globjects::ref_ptr<glat::AbstractState> glat::PathState::clone() {
	throw std::logic_error("The method or operation is not implemented.");
}

void glat::PathState::setExternalReference(const globjects::ref_ptr<glat::AbstractExternalReference>& reference) {
	ReferenceableState::setExternalReference(reference);
	m_externalReference->setupExternalReference(*this);
}
