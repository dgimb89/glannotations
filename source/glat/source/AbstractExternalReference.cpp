#include <glat/AbstractExternalReference.h>
#include <glat/AbstractRenderer.h>
#include <stdexcept>

void glat::AbstractExternalReference::setPositioningOnly(bool positioningOnly) {
	setDirty(true);
	m_positioningOnly = positioningOnly;
}

glat::AbstractExternalReference::AbstractExternalReference(bool positioningOnly) {
	setPositioningOnly(positioningOnly);
}

bool glat::AbstractExternalReference::isPositioningOnly() {
	return m_positioningOnly;
}

void glat::AbstractExternalReference::setupExternalReference(const InternalState& state) {
	assertNotReused();
}

void glat::AbstractExternalReference::setupExternalReference(const PathState& state) {
	assertNotReused();
}

void glat::AbstractExternalReference::assertNotReused() {
	if (!isDirty()) {
		throw std::runtime_error("External References can not be reused for different annotations!");
	}
}

inline void glat::AbstractExternalReference::setBindingIndex(unsigned int bindingIndex) {
	m_bindingIndex = bindingIndex;
}

inline unsigned int glat::AbstractExternalReference::getBindingIndex() const {
	return m_bindingIndex;
}

void glat::AbstractExternalReference::updateBindings(const glat::AbstractRenderer& renderer) {
	if (isDirty()) {
		setBindingIndex(renderer.getMatricesBindingIndex());
		m_externalPrimitive->setBindingIndex(getBindingIndex());
	}
}

void glat::AbstractExternalReference::draw() {
	if (!isPositioningOnly()) {
		m_externalPrimitive->draw();
	}
	setDirty(false);
}

void glat::AbstractExternalReference::setDirty(bool dirtyValue) const  {
	DirtyFlagObject::setDirty(dirtyValue);
}

void glat::AbstractExternalReference::setColor(glm::vec4 color) {
	m_externalPrimitive->setColor(color);
}
