#pragma once

#include <glannotations/AbstractState.h>
#include <glannotations/AbstractExternalReference.h>

namespace glat {
	class GLANNOTATIONS_API ReferenceableState : public glat::AbstractState {
	public:
		virtual void setExternalReference(const globjects::ref_ptr<glat::AbstractExternalReference>& reference);
		globjects::ref_ptr<glat::AbstractExternalReference> getExternalReference() const;

	protected:
		void copyState(ReferenceableState& copyTo) const;

		globjects::ref_ptr<glat::AbstractExternalReference> m_externalReference = nullptr;
	};
}