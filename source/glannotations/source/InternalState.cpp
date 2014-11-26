#include <glannotations/InternalState.h>
#include <glannotations/AbstractRenderer.h>
#include <glannotations/AbstractAnnotation.h>
#include <glannotations/StateInterpolation.h>

glat::InternalState::InternalState(glm::vec3 ll, glm::vec3 lr, glm::vec3 ur) {
	setExtends(ll, lr, ur);
}

void glat::InternalState::setExtends(glm::vec3 ll, glm::vec3 lr, glm::vec3 ur) {
	if (ll != m_ll || lr != m_lr || ur != m_ur) {
		setDirty(true);
		m_ll = ll;
		m_lr = lr;
		m_ur = ur;
	}
}

const glm::vec3& glat::InternalState::getLL() const {
	return m_ll;
}

const glm::vec3& glat::InternalState::getLR() const {
	return m_lr;
}


const glm::vec3& glat::InternalState::getUR() const {
	return m_ur;
}

void glat::InternalState::draw(const AbstractRenderer& renderer) {
	if (m_externalReference) {
		m_externalReference->updatePositioning(*this);
		m_externalReference->updateBindings(renderer);
		m_externalReference->draw();
	}
	renderer.drawSetupState(*this);
}

bool glat::InternalState::isValid() const {
	return (m_ur - m_ll).length() > 0;
}

globjects::ref_ptr<glat::AbstractState> glat::InternalState::interpolateWith(const InternalState& mixState, float mix) {
	return glat::Interpolation::interpolate(*this, mixState, mix);
}

globjects::ref_ptr<glat::AbstractState> glat::InternalState::interpolateWith(const PathState& mixState, float mix) {
	return glat::Interpolation::interpolate(*this, mixState, mix);
}

globjects::ref_ptr<glat::AbstractState> glat::InternalState::interpolateWith(const ViewportState& mixState, float mix) {
	return glat::Interpolation::interpolate(*this, mixState, mix);
}

globjects::ref_ptr<glat::AbstractState> glat::InternalState::clone() const {
	globjects::ref_ptr<glat::AbstractState> clonedState(new InternalState(m_ll, m_lr, m_ur));
	ReferenceableState::copyState(dynamic_cast<ReferenceableState&> (*clonedState));
	return clonedState;
}

void glat::InternalState::setExternalReference(const globjects::ref_ptr<glat::AbstractExternalReference>& reference) {
	ReferenceableState::setExternalReference(reference);
	m_externalReference->setupExternalReference(*this);
}

glat::BoundingBox glat::InternalState::getBoundingBox() {
	glat::BoundingBox bb;
	bb.extendBy(m_ll);
	bb.extendBy(m_lr);
	bb.extendBy(m_ur);
	return bb;
}
