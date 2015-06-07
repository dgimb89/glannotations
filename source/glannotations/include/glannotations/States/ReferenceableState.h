#pragma once

#include <glannotations/States/AbstractState.h>
#include <glannotations/Externals/AbstractExternalReference.h>

namespace glannotations {
	class GLANNOTATIONS_API ReferenceableState : public glannotations::AbstractState {
	public:
		virtual void setExternalReference(const globjects::ref_ptr<glannotations::AbstractExternalReference>& reference);
		globjects::ref_ptr<glannotations::AbstractExternalReference> getExternalReference() const;
		bool getAutoUpdate() const;
		void setAutoUpdate(bool val);
		/*!
		 *	\brief		Allows manual positioning update. Only Use when AutoUpdate is set to false!
		 */
		template <class T>
		void updatePositioning(T& manipulatedState);

	protected:
		void copyState(ReferenceableState& copyTo) const;

		template <class T>
		void drawExternalReference(const AbstractRenderer& renderer, T& manipulatedState);

		bool m_autoUpdate = true;
		globjects::ref_ptr<glannotations::AbstractExternalReference> m_externalReference = nullptr;
	};
}

#include <glannotations/States/ReferenceableState.hpp>