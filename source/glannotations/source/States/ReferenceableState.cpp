#include <glannotations/States/ReferenceableState.h>

void glannotations::ReferenceableState::setExternalReference(const globjects::ref_ptr<glannotations::AbstractExternalReference>& reference) {
	setDirty(true);
	m_externalReference = reference;
}

globjects::ref_ptr<glannotations::AbstractExternalReference> glannotations::ReferenceableState::getExternalReference() const {
	return m_externalReference;
}

void glannotations::ReferenceableState::copyState(ReferenceableState& copyTo) const {
	if (getExternalReference()) {
		copyTo.m_externalReference = getExternalReference(); // we set the data directly to avoid reinitialization
	}
	AbstractState::copyState(copyTo);
}
