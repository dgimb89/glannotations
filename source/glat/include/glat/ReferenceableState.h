#pragma once

#include <glat/AbstractState.h>
#include <glat/AbstractExternalReference.h>

namespace glat {
	class ReferenceableState : public glat::AbstractState {
	public:
		void setExternalReference(const glow::ref_ptr<glat::AbstractExternalReference>& reference);
		glow::ref_ptr<glat::AbstractExternalReference> getExternalReference() const;

	protected:
		void copyState(ReferenceableState& copyTo) const;
		glow::ref_ptr<glat::AbstractExternalReference> m_externalReference = nullptr;
	};
}