#ifndef GLAT_ABSTRACT_PRIMITIVE_RENDERER_H
#define GLAT_ABSTRACT_PRIMITIVE_RENDERER_H

#include <glat/AbstractDrawingPrimitive.h>
#include <glat/AbstractRenderer.h>
#include <glat/Styling.h>

#include <glow/ref_ptr.h>

namespace glat {
	class AbstractPrimitiveRenderer : public AbstractRenderer {
	protected:

		void setupOutline(const glat::Styling* outline);
		void setupBumpMap(const glat::Styling* bumpMap);

		virtual void drawSetupState(const glat::ViewportState& state) const;
		virtual void drawSetupState(const glat::InternalState& state) const;
		virtual void drawSetupState(const glat::ExternalBoxState& state) const;

		// we set the primitive to mutable because there is no outside interference at all - the quad is JUST used for rendering for this specific Renderer
		mutable glow::ref_ptr<glat::AbstractDrawingPrimitive> m_drawingPrimitive;
	};
}

#endif //! GLAT_ABSTRACT_PRIMITIVE_RENDERER_H