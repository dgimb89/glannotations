#pragma once

#include <glat/DirtyFlagObject.h>

namespace glat {

	// forward declaration 
	class AbstractAnnotation;
	class ViewportState;
	class InternalState;

	class AbstractRenderer : public glat::DirtyFlagObject {
		friend class ViewportState;
		friend class InternalState;
		friend class ExternalBoxState;
	public:
		virtual void draw(glat::AbstractAnnotation* annotation) = 0;

	protected:
		virtual void drawSetupState(const glat::ViewportState& state) const = 0;
		virtual void drawSetupState(const glat::InternalState& state) const = 0;
		virtual void drawSetupState(const glat::ExternalBoxState& state) const = 0;
	};
}