#pragma once

#include <glat/AbstractExternalReference.h>
#include <glat/Prismoid.h>
#include <glat/glat_api.h>

namespace glat {
	class GLAT_API LabelReference : public glat::AbstractExternalReference {
	public:
	protected:
		virtual void draw() override;
		virtual void updatePositioning(InternalState& state) override;
		virtual void updatePositioning(PathState& state) override;

		virtual void updateBindings(const glat::AbstractRenderer& renderer) override;
	
	private:
		glow::ref_ptr<glat::Prismoid> m_prismoid;
	};
}