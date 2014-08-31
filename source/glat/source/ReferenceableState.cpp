#include <glat/ReferenceableState.h>

void glat::ReferenceableState::setExternalReference(const glo::ref_ptr<glat::AbstractExternalReference>& reference) {
	setDirty(true);
	m_externalReference = reference;
}

glo::ref_ptr<glat::AbstractExternalReference> glat::ReferenceableState::getExternalReference() const {
	return m_externalReference;
}

void glat::ReferenceableState::copyState(ReferenceableState& copyTo) const {
	if (getExternalReference()) {
		copyTo.m_externalReference = getExternalReference(); // we set the data directly to avoid reinitialization
	}
	AbstractState::copyState(copyTo);
}
