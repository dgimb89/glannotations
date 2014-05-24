#include <glat/InternalPathState.h>
#include <glat/AbstractRenderer.h>
#include <glat/AbstractAnnotation.h>


bool glat::InternalPathState::isValid() {
	throw std::logic_error("The method or operation is not implemented.");
}

void glat::InternalPathState::interpolate(const AbstractAnnotation& annotation, AbstractState* secondState, float interpolate) const {
	throw std::logic_error("The method or operation is not implemented.");
}

void glat::InternalPathState::interpolate(const AbstractAnnotation& annotation, const ViewportState& viewState, float interpolate) const {
	throw std::logic_error("The method or operation is not implemented.");
}

void glat::InternalPathState::interpolate(const AbstractAnnotation& annotation, const InternalState& internalState, float interpolate) const {
	throw std::logic_error("The method or operation is not implemented.");
}

void glat::InternalPathState::interpolate(const AbstractAnnotation& annotation, const ExternalBoxState& externalState, float interpolate) const {
	throw std::logic_error("The method or operation is not implemented.");
}

void glat::InternalPathState::draw(const AbstractRenderer& renderer) const {
	renderer.drawSetupState(*this);
}
