#include <glannotations/States/QuadState.h>
#include <glannotations/Renderer/AbstractRenderer.h>
#include <glannotations/AbstractAnnotation.h>
#include <glannotations/States/StateInterpolation.h>

glannotations::QuadState::QuadState(glm::vec3 ll, glm::vec3 lr, glm::vec3 ur) {
	setExtends(ll, lr, ur);
}

void glannotations::QuadState::setExtends(glm::vec3 ll, glm::vec3 lr, glm::vec3 ur) {
	if (ll != m_ll || lr != m_lr || ur != m_ur) {
		setDirty(true);
		m_ll = ll;
		m_lr = lr;
		m_ur = ur;
	}
}

const glm::vec3& glannotations::QuadState::getLL() const {
	return m_ll;
}

const glm::vec3& glannotations::QuadState::getLR() const {
	return m_lr;
}


const glm::vec3& glannotations::QuadState::getUR() const {
	return m_ur;
}

void glannotations::QuadState::draw(const AbstractRenderer& renderer) {
	drawExternalReference(renderer, *this);
	renderer.drawSetupState(*this);
}

bool glannotations::QuadState::isValid() const {
	return (m_ur - m_ll).length() > 0;
}

globjects::ref_ptr<glannotations::AbstractState> glannotations::QuadState::interpolateWith(const QuadState& mixState, float mix) {
	return glannotations::Interpolation::interpolate(*this, mixState, mix);
}

globjects::ref_ptr<glannotations::AbstractState> glannotations::QuadState::interpolateWith(const SplineState& mixState, float mix) {
	return glannotations::Interpolation::interpolate(*this, mixState, mix);
}

globjects::ref_ptr<glannotations::AbstractState> glannotations::QuadState::interpolateWith(const ViewportState& mixState, float mix) {
	return glannotations::Interpolation::interpolate(*this, mixState, mix);
}

globjects::ref_ptr<glannotations::AbstractState> glannotations::QuadState::clone() const {
	globjects::ref_ptr<glannotations::AbstractState> clonedState(new QuadState(m_ll, m_lr, m_ur));
	ReferenceableState::copyState(dynamic_cast<ReferenceableState&> (*clonedState));
	return clonedState;
}

void glannotations::QuadState::setExternalReference(const globjects::ref_ptr<glannotations::AbstractExternalReference>& reference) {
	ReferenceableState::setExternalReference(reference);
	m_externalReference->setupExternalReference(*this);
}

glannotations::BoundingBox glannotations::QuadState::getBoundingBox() {
	glannotations::BoundingBox bb;
	bb.extendBy(m_ll);
	bb.extendBy(m_lr);
	bb.extendBy(m_ur);
	return bb;
}

void glannotations::QuadState::updateExtends(glm::vec2 sourceExtends) {
	cropExtends(m_ll, m_lr, m_ur, sourceExtends);
}

void glannotations::QuadState::prepare() {
	updatePositioning(*this);
	AbstractState::prepare();
}
