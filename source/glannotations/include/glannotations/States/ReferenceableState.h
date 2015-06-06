#pragma once

#include <glannotations/States/AbstractState.h>
#include <glannotations/Externals/AbstractExternalReference.h>

namespace glannotations {
	class GLANNOTATIONS_API ReferenceableState : public glannotations::AbstractState {
	public:
		virtual void setExternalReference(const globjects::ref_ptr<glannotations::AbstractExternalReference>& reference);
		globjects::ref_ptr<glannotations::AbstractExternalReference> getExternalReference() const;

	protected:
		void copyState(ReferenceableState& copyTo) const;

		globjects::ref_ptr<glannotations::AbstractExternalReference> m_externalReference = nullptr;
	};
}