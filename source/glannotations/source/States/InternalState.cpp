#include <glannotations/States/InternalState.h>
#include <glannotations/Renderer/AbstractRenderer.h>
#include <glannotations/AbstractAnnotation.h>
#include <glannotations/States/StateInterpolation.h>

glannotations::InternalState::InternalState(glm::vec3 ll, glm::vec3 lr, glm::vec3 ur) {
	setExtends(ll, lr, ur);
}

void glannotations::InternalState::setExtends(glm::vec3 ll, glm::vec3 lr, glm::vec3 ur) {
	if (ll != m_ll || lr != m_lr || ur != m_ur) {
		setDirty(true);
		m_ll = ll;
		m_lr = lr;
		m_ur = ur;
	}
}

const glm::vec3& glannotations::InternalState::getLL() const {
	return m_ll;
}

const glm::vec3& glannotations::InternalState::getLR() const {
	return m_lr;
}


const glm::vec3& glannotations::InternalState::getUR() const {
	return m_ur;
}

void glannotations::InternalState::draw(const AbstractRenderer& renderer) {
	if (m_externalReference) {
		m_externalReference->updateBindings(renderer);
		m_externalReference->updatePositioning(*this);
		m_externalReference->draw();
	}
	renderer.drawSetupState(*this);
}

bool glannotations::InternalState::isValid() const {
	return (m_ur - m_ll).length() > 0;
}

globjects::ref_ptr<glannotations::AbstractState> glannotations::InternalState::interpolateWith(const InternalState& mixState, float mix) {
	return glannotations::Interpolation::interpolate(*this, mixState, mix);
}

globjects::ref_ptr<glannotations::AbstractState> glannotations::InternalState::interpolateWith(const SplineState& mixState, float mix) {
	return glannotations::Interpolation::interpolate(*this, mixState, mix);
}

globjects::ref_ptr<glannotations::AbstractState> glannotations::InternalState::interpolateWith(const ViewportState& mixState, float mix) {
	return glannotations::Interpolation::interpolate(*this, mixState, mix);
}

globjects::ref_ptr<glannotations::AbstractState> glannotations::InternalState::clone() const {
	globjects::ref_ptr<glannotations::AbstractState> clonedState(new InternalState(m_ll, m_lr, m_ur));
	ReferenceableState::copyState(dynamic_cast<ReferenceableState&> (*clonedState));
	return clonedState;
}

void glannotations::InternalState::setExternalReference(const globjects::ref_ptr<glannotations::AbstractExternalReference>& reference) {
	ReferenceableState::setExternalReference(reference);
	m_externalReference->setupExternalReference(*this);
}

glannotations::BoundingBox glannotations::InternalState::getBoundingBox() {
	glannotations::BoundingBox bb;
	bb.extendBy(m_ll);
	bb.extendBy(m_lr);
	bb.extendBy(m_ur);
	return bb;
}

void glannotations::InternalState::updateExtends(glm::vec2 sourceExtends) {
	cropExtends(m_ll, m_lr, m_ur, sourceExtends);
}
