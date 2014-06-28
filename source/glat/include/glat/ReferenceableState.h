#pragma once

#include <glat/AbstractState.h>
#include <glat/AbstractExternalReference.h>

namespace glat {
	class GLAT_API ReferenceableState : public glat::AbstractState {
	public:
		virtual void setExternalReference(const glow::ref_ptr<glat::AbstractExternalReference>& reference);
		glow::ref_ptr<glat::AbstractExternalReference> getExternalReference() const;
		const glm::mat4& getViewProjection() const;

	protected:
		void copyState(ReferenceableState& copyTo) const;

		glow::ref_ptr<glat::AbstractExternalReference> m_externalReference = nullptr;
	};
}