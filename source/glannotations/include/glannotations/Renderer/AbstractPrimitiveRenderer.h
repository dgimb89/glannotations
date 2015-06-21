#pragma once

#include <globjects/base/ref_ptr.h>

#include <glannotations/Renderer/AbstractTexturedPrimitive.h>
#include <glannotations/Renderer/QuadStrip.h>
#include <glannotations/Renderer/BendedQuadStrip.h>
#include <glannotations/Renderer/AbstractRenderer.h>
#include <glannotations/Styles/Styling.h>

namespace glannotations {
	class AbstractPrimitiveRenderer : public AbstractRenderer {
	protected:
		AbstractPrimitiveRenderer(gl::GLuint globalMatricesBindingIndex);
		void setupOutline(const glannotations::Styling* outline) const;
		void setupBumpMap(const glannotations::Styling* bumpMap) const;

		virtual void drawSetupState(const globjects::ref_ptr<glannotations::AbstractAnnotation>& annotation, glannotations::ViewportState& state) const override;
		virtual void drawSetupState(const globjects::ref_ptr<glannotations::AbstractAnnotation>& annotation, glannotations::QuadState& state) const override;
		virtual void drawSetupState(const globjects::ref_ptr<glannotations::AbstractAnnotation>& annotation, glannotations::SplineState& state) const override;


		// we set the primitive to mutable because there is no outside interference at all - the quad is JUST used for rendering for this specific Renderer
		mutable globjects::ref_ptr<glannotations::AbstractTexturedPrimitive> m_drawingPrimitive;
	};
}