#include <glannotations/PathState.h>
#include <glannotations/AbstractRenderer.h>
#include <glannotations/AbstractAnnotation.h>
#include <glannotations/StateInterpolation.h>


bool glannotations::PathState::isValid() {
	throw std::logic_error("The method or operation is not implemented.");
}

void glannotations::PathState::draw(const AbstractRenderer& renderer) {
	if (m_externalReference) {
		m_externalReference->updatePositioning(*this);
		m_externalReference->draw();
	}
	renderer.drawSetupState(*this);
}

globjects::ref_ptr<glannotations::AbstractState> glannotations::PathState::interpolateWith(const InternalState& mixState, float mix) {
	return glannotations::Interpolation::interpolate(*this, mixState, mix);
}

globjects::ref_ptr<glannotations::AbstractState> glannotations::PathState::interpolateWith(const PathState& mixState, float mix) {
	return glannotations::Interpolation::interpolate(*this, mixState, mix);
}

globjects::ref_ptr<glannotations::AbstractState> glannotations::PathState::interpolateWith(const ViewportState& mixState, float mix) {
	return glannotations::Interpolation::interpolate(*this, mixState, mix);
}

globjects::ref_ptr<glannotations::AbstractState> glannotations::PathState::clone() {
	throw std::logic_error("The method or operation is not implemented.");
}

void glannotations::PathState::setExternalReference(const globjects::ref_ptr<glannotations::AbstractExternalReference>& reference) {
	ReferenceableState::setExternalReference(reference);
	m_externalReference->setupExternalReference(*this);
}
