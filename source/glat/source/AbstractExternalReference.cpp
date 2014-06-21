#include <glat/AbstractExternalReference.h>
#include <stdexcept>

void glat::AbstractExternalReference::setCamera(glowutils::Camera* camera) {
	setDirty(true);
	m_camera = camera;
}

void glat::AbstractExternalReference::setPositioningOnly(bool positioningOnly) {
	setDirty(true);
	m_positioningOnly = positioningOnly;
}

glat::AbstractExternalReference::AbstractExternalReference(glowutils::Camera* camera, bool positioningOnly) {
	setCamera(camera);
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
