#include <glat/AbstractExternalReference.h>

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
