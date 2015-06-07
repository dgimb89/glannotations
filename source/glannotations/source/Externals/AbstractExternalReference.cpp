#include <glannotations/Externals/AbstractExternalReference.h>
#include <glannotations/Renderer/AbstractRenderer.h>
#include <stdexcept>

void glannotations::AbstractExternalReference::setPositioningOnly(bool positioningOnly) {
	setDirty(true);
	m_positioningOnly = positioningOnly;
}

glannotations::AbstractExternalReference::AbstractExternalReference(bool positioningOnly) {
	setPositioningOnly(positioningOnly);
}

bool glannotations::AbstractExternalReference::isPositioningOnly() const {
	return m_positioningOnly;
}

void glannotations::AbstractExternalReference::setupExternalReference(const InternalState& state) {
	assertNotReused();
}

void glannotations::AbstractExternalReference::setupExternalReference(const SplineState& state) {
	assertNotReused();
}

void glannotations::AbstractExternalReference::assertNotReused() {
	if (!isDirty()) {
		throw std::runtime_error("External References can not be reused for different annotations!");
	}
}

void glannotations::AbstractExternalReference::setBindingIndex(unsigned int bindingIndex) {
	m_bindingIndex = bindingIndex;
}

unsigned int glannotations::AbstractExternalReference::getBindingIndex() const {
	return m_bindingIndex;
}

void glannotations::AbstractExternalReference::updateBindings(const glannotations::AbstractRenderer& renderer) {
	setBindingIndex(renderer.getMatricesBindingIndex());
	if (!isPositioningOnly()) {
		m_externalPrimitive->setBindingIndex(getBindingIndex());
	}
	m_bindingIndexSet = true;
}

void glannotations::AbstractExternalReference::draw() {
	if (!isPositioningOnly()) {
		if (isDirty()) {
			m_externalPrimitive->setColor(m_color);
		}
		m_externalPrimitive->draw();
	}
	setDirty(false);
}

void glannotations::AbstractExternalReference::setDirty(bool dirtyValue) const  {
	// changing visbility to allow checking dirtyness for reusage-check
	DirtyFlagObject::setDirty(dirtyValue);
}

void glannotations::AbstractExternalReference::setColor(glm::vec4 color) {
	setDirty(true);
	m_color = color;
}

bool glannotations::AbstractExternalReference::bindingIndexSet() const {
	return m_bindingIndexSet;
}
