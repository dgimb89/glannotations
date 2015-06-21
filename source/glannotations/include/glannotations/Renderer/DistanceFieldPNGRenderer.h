#pragma once

#include <string>

#include <glannotations/Renderer/AbstractPrimitiveRenderer.h>

namespace glannotations {
	class PNGAnnotation;

	class DistanceFieldPNGRenderer : public AbstractPrimitiveRenderer {
	public:
		DistanceFieldPNGRenderer(gl::GLuint globalMatricesBindingIndex);
		virtual void prepare(const globjects::ref_ptr<glannotations::AbstractAnnotation>& annotation);
		virtual void draw(const globjects::ref_ptr<glannotations::AbstractAnnotation>& annotation);

	protected:
		void setupStylings(PNGAnnotation* annotation) const;
		void prepareQuadStrip(PNGAnnotation* annotation) const;
		void prepareSpline(PNGAnnotation* annotation) const;

		virtual void drawSetupState(const globjects::ref_ptr<glannotations::AbstractAnnotation>& annotation, glannotations::ViewportState& state) const override;
		virtual void drawSetupState(const globjects::ref_ptr<glannotations::AbstractAnnotation>& annotation, glannotations::QuadState& state) const override;
		virtual void drawSetupState(const globjects::ref_ptr<glannotations::AbstractAnnotation>& annotation, glannotations::SplineState& state) const override;

	private:
		std::shared_ptr<globjects::Texture> m_texture;
	};
}