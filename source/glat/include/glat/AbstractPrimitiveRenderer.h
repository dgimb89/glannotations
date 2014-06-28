#pragma once

#include <glowbase/ref_ptr.h>

#include <glat/AbstractTexturedPrimitive.h>
#include <glat/AbstractRenderer.h>
#include <glat/Styling.h>

namespace glat {
	class AbstractPrimitiveRenderer : public AbstractRenderer {
	protected:
		AbstractPrimitiveRenderer(gl::GLuint globalMatricesBindingIndex);
		void setupOutline(const glat::Styling* outline);
		void setupBumpMap(const glat::Styling* bumpMap);

		virtual void drawSetupState(const glat::ViewportState& state) const override;
		virtual void drawSetupState(const glat::InternalState& state) const override;
		virtual void drawSetupState(const glat::PathState& state) const override;


		// we set the primitive to mutable because there is no outside interference at all - the quad is JUST used for rendering for this specific Renderer
		mutable glow::ref_ptr<glat::AbstractTexturedPrimitive> m_drawingPrimitive;
	};
}