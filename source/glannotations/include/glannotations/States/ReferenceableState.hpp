#pragma once

#include <glannotations/States/ReferenceableState.h>


template <class T>
void glannotations::ReferenceableState::updatePositioning(T& manipulatedState) {
	if(!m_externalReference) return;
	m_externalReference->updatePositioning(manipulatedState);
}

template <class T>
void glannotations::ReferenceableState::drawExternalReference(const AbstractRenderer& renderer, T& manipulatedState) {
	if (!m_externalReference) return;
	if (isDirty()) {
		m_externalReference->updateBindings(renderer);
	}
	if (isDirty() || getAutoUpdate()) {
		updatePositioning(manipulatedState);
	}
	m_externalReference->draw();
}