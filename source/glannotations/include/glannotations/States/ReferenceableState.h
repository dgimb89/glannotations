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

		/*!
		*	\brief		Allows manual positioning update. Only Use when AutoUpdate is set to false!
		*/
		template <class T>
		void updatePositioning(T& manipulatedState);

		template <class T>
		void drawExternalReference(const AbstractRenderer& renderer, T& manipulatedState);

		globjects::ref_ptr<glannotations::AbstractExternalReference> m_externalReference = nullptr;
	};
}

#include <glannotations/States/ReferenceableState.hpp>