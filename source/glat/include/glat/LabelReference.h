#pragma once

#include <glat/AbstractExternalReference.h>
#include <glat/glat_api.h>

namespace glat {
	class GLAT_API LabelReference : public glat::AbstractExternalReference {
	public:
		virtual void draw();
		virtual void updatePositioning(InternalState& state);
		virtual void updatePositioning(InternalPathState& state);
	};
}