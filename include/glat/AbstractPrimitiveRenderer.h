#pragma once

#include <glat/AbstractTexturedPrimitive.h>
#include <glat/AbstractRenderer.h>
#include <glat/Styling.h>

#include <glow/ref_ptr.h>

namespace glat {
	class AbstractPrimitiveRenderer : public AbstractRenderer {
	protected:

		void setupOutline(const glat::Styling* outline);
		void setupBumpMap(const glat::Styling* bumpMap);

		virtual void drawSetupState(const glat::ViewportState& state) const override;
		virtual void drawSetupState(const glat::InternalState& state) const override;
		virtual void drawSetupState(const glat::ExternalBoxState& state) const override;
		virtual void drawSetupState(const glat::ExternalLabelState& state) const override;

		virtual void drawGenericExternalState(const glat::AbstractExternalState& state) const;

		virtual void draw(glat::AbstractAnnotation* annotation) override;

		// we set the primitive to mutable because there is no outside interference at all - the quad is JUST used for rendering for this specific Renderer
		mutable glow::ref_ptr<glat::AbstractTexturedPrimitive> m_drawingPrimitive;
	};
}