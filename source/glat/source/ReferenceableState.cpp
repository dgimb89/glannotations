#include <glat/ReferenceableState.h>

void glat::ReferenceableState::setExternalReference(const glow::ref_ptr<glat::AbstractExternalReference>& reference) {
	m_externalReference = reference;
}

glow::ref_ptr<glat::AbstractExternalReference> glat::ReferenceableState::getExternalReference() const {
	return m_externalReference;
}

void glat::ReferenceableState::copyState(ReferenceableState& copyTo) const {
	if (getExternalReference()) {
		copyTo.m_externalReference = getExternalReference(); // we set the data directly to avoid reinitialization
	}
	AbstractState::copyState(copyTo);
}
