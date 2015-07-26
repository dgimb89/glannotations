#pragma once
#include <globjects/base/ref_ptr.h>

#include <glannotations/Renderer/AbstractPrimitiveRenderer.h>
#include <glannotations/Utility/GlyphSetConfig.h>

namespace glannotations {
	class FontAnnotation;

	class DistanceFieldFontRenderer : public glannotations::AbstractPrimitiveRenderer {
	public:
		DistanceFieldFontRenderer(gl::GLuint matricesBindingIndex);
		virtual void prepare(const globjects::ref_ptr<glannotations::AbstractAnnotation>& annotation);
		virtual void draw(const globjects::ref_ptr<glannotations::AbstractAnnotation>& annotation);
		virtual void directDrawCall();

	protected:
		void setupStylings(FontAnnotation* annotation) const;
		void prepareQuadStrip(FontAnnotation* annotation) const;
		void prepareSpline(FontAnnotation* annotation) const;

		virtual void drawSetupState(const globjects::ref_ptr<glannotations::AbstractAnnotation>& annotation, glannotations::ViewportState& state) const override;
		virtual void drawSetupState(const globjects::ref_ptr<glannotations::AbstractAnnotation>& annotation, glannotations::QuadState& state) const override;
		virtual void drawSetupState(const globjects::ref_ptr<glannotations::AbstractAnnotation>& annotation, glannotations::SplineState& state) const override;

	private:
		globjects::ref_ptr<glannotations::GlyphSetConfig> m_glyphConfig = nullptr;
		std::shared_ptr<globjects::Texture> m_texture;
	};
}
