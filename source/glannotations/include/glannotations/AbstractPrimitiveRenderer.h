#pragma once

#include <globjects/base/ref_ptr.h>

#include <glannotations/AbstractTexturedPrimitive.h>
#include <glannotations/QuadStrip.h>
#include <glannotations/BendedQuadStrip.h>
#include <glannotations/AbstractRenderer.h>
#include <glannotations/Styling.h>

namespace glannotations {
	class AbstractPrimitiveRenderer : public AbstractRenderer {
	protected:
		AbstractPrimitiveRenderer(gl::GLuint globalMatricesBindingIndex);
		void setupOutline(const glannotations::Styling* outline);
		void setupBumpMap(const glannotations::Styling* bumpMap);

		virtual void drawSetupState(glannotations::ViewportState& state) const override;
		virtual void drawSetupState(glannotations::InternalState& state) const override;
		virtual void drawSetupState(glannotations::SplineState& state) const override;


		// we set the primitive to mutable because there is no outside interference at all - the quad is JUST used for rendering for this specific Renderer
		mutable globjects::ref_ptr<glannotations::AbstractTexturedPrimitive> m_drawingPrimitive;
	};
}