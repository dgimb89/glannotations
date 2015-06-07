#pragma once

#include <glannotations/States/ReferenceableState.h>


template <class T>
void glannotations::ReferenceableState::updatePositioning(T& manipulatedState) {
	if(!m_externalReference || !m_externalReference->bindingIndexSet()) return;
	m_externalReference->updatePositioning(manipulatedState);
}

template <class T>
void glannotations::ReferenceableState::drawExternalReference(const AbstractRenderer& renderer, T& manipulatedState) {
	if (!m_externalReference) return;
	if (!m_externalReference->bindingIndexSet()) {
		m_externalReference->updateBindings(renderer);
		// we update positioning directly after settings bindings in case autoupdate is already off but we need initial positioning
		m_externalReference->updatePositioning(manipulatedState);
	}
	if (getAutoUpdate()) {
		m_externalReference->updatePositioning(manipulatedState);
	}
	m_externalReference->draw();
}