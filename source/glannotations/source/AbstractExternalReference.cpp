#include <glannotations/AbstractExternalReference.h>
#include <glannotations/AbstractRenderer.h>
#include <stdexcept>

void glannotations::AbstractExternalReference::setPositioningOnly(bool positioningOnly) {
	setDirty(true);
	m_positioningOnly = positioningOnly;
}

glannotations::AbstractExternalReference::AbstractExternalReference(bool positioningOnly) {
	setPositioningOnly(positioningOnly);
}

bool glannotations::AbstractExternalReference::isPositioningOnly() {
	return m_positioningOnly;
}

void glannotations::AbstractExternalReference::setupExternalReference(const InternalState& state) {
	assertNotReused();
}

void glannotations::AbstractExternalReference::setupExternalReference(const PathState& state) {
	assertNotReused();
}

void glannotations::AbstractExternalReference::assertNotReused() {
	if (!isDirty()) {
		throw std::runtime_error("External References can not be reused for different annotations!");
	}
}

inline void glannotations::AbstractExternalReference::setBindingIndex(unsigned int bindingIndex) {
	m_bindingIndex = bindingIndex;
}

inline unsigned int glannotations::AbstractExternalReference::getBindingIndex() const {
	return m_bindingIndex;
}

void glannotations::AbstractExternalReference::updateBindings(const glannotations::AbstractRenderer& renderer) {
	if (isDirty()) {
		setBindingIndex(renderer.getMatricesBindingIndex());
		if (!isPositioningOnly()) {
			m_externalPrimitive->setBindingIndex(getBindingIndex());
		}
	}
}

void glannotations::AbstractExternalReference::draw() {
	if (!isPositioningOnly()) {
		m_externalPrimitive->draw();
	}
	setDirty(false);
}

void glannotations::AbstractExternalReference::setDirty(bool dirtyValue) const  {
	DirtyFlagObject::setDirty(dirtyValue);
}

void glannotations::AbstractExternalReference::setColor(glm::vec4 color) {
	m_externalPrimitive->setColor(color);
}
