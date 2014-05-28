#pragma once

#include <glowbase/Referenced.h>
#include <glowbase/ref_ptr.h>

#include <glat/AbstractDrawingPrimitive.h>
#include <glat/glat_api.h>

namespace glat {

	class InternalState;
	class InternalPathState;

	class GLAT_API AbstractExternalReference : public glow::Referenced {
	public:
		virtual void draw();

		// updates positioning of given state
		virtual void updatePositioning(InternalState& state) = 0;
		virtual void updatePositioning(InternalPathState& state) = 0;

	protected:
		glow::ref_ptr<glat::AbstractDrawingPrimitive> m_externalPrimitives;
	};
}