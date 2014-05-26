#pragma once

#include <glowbase/Referenced.h>

#include <glat/glat_api.h>

namespace glat {

	class InternalState;
	class InternalPathState;

	class GLAT_API AbstractExternalReference : public glow::Referenced {
	public:
		virtual void draw() = 0;

		// updates positioning of given state
		virtual void updatePositioning(InternalState& state) = 0;
		virtual void updatePositioning(InternalPathState& state) = 0;
	};
}