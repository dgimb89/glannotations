#define __STDC_LIMIT_MACROS

#include <glbinding/gl/bitfield.h>
#include <glbinding/gl/functions.h>

#include <glannotations/Renderer/DistanceFieldFontRenderer.h>
#include <glannotations/States/ViewportState.h>
#include <glannotations/States/QuadState.h>
#include <glannotations/FontAnnotation.h>
#include <glannotations/Common/TextureManager.h>
#include <glannotations/Renderer/QuadStrip.h>
#include <glannotations/Renderer/SplineQuads.h>

void glannotations::DistanceFieldFontRenderer::draw(const globjects::ref_ptr<glannotations::AbstractAnnotation>& annotation) {
	if (annotation->isDirty()) {
		prepare(annotation);
	}
	gl::glEnable(gl::GL_BLEND);
	gl::glBlendFunc(gl::GL_SRC_ALPHA, gl::GL_ONE_MINUS_SRC_ALPHA);
	annotation->getRenderState()->draw(annotation, *this);
	gl::glDisable(gl::GL_BLEND);
}

glannotations::DistanceFieldFontRenderer::DistanceFieldFontRenderer(gl::GLuint matricesBindingIndex) : AbstractPrimitiveRenderer(matricesBindingIndex) {
}

void glannotations::DistanceFieldFontRenderer::prepare(const globjects::ref_ptr<glannotations::AbstractAnnotation>& annotation) {
	FontAnnotation* currentAnnotation = dynamic_cast<FontAnnotation*>(annotation.get());
	m_glyphConfig = new glannotations::GlyphSetConfig(currentAnnotation->getFontName());
	m_texture = glannotations::TextureManager::getInstance().getTexture(m_glyphConfig->getGlyphsetImageName());
}

void glannotations::DistanceFieldFontRenderer::drawSetupState(const globjects::ref_ptr<glannotations::AbstractAnnotation>& annotation, glannotations::ViewportState& state) const  {
	if (annotation->isDirty()) {
		prepareQuadStrip(dynamic_cast<FontAnnotation*>(annotation.get()));
	}

	AbstractPrimitiveRenderer::drawSetupState(annotation, state);
}

void glannotations::DistanceFieldFontRenderer::drawSetupState(const globjects::ref_ptr<glannotations::AbstractAnnotation>& annotation, glannotations::QuadState& state) const  {
	if (annotation->isDirty()) {
		prepareQuadStrip(dynamic_cast<FontAnnotation*>(annotation.get()));
	}

	AbstractPrimitiveRenderer::drawSetupState(annotation, state);
}

void glannotations::DistanceFieldFontRenderer::drawSetupState(const globjects::ref_ptr<glannotations::AbstractAnnotation>& annotation, glannotations::SplineState& state) const  {
	if (annotation->isDirty() || state.isSplineDirty()) {
		prepareSpline(dynamic_cast<FontAnnotation*>(annotation.get()));
		state.setSplineDirty(false);
	}

	AbstractPrimitiveRenderer::drawSetupState(annotation, state);
}

void glannotations::DistanceFieldFontRenderer::setupStylings(FontAnnotation* annotation) const {
	m_drawingPrimitive->setColor(annotation->getColor());
	setupOutline(annotation->getRenderState()->getStyling("Outline"));
	setupBumpMap(annotation->getRenderState()->getStyling("BumpMap"));
}

void glannotations::DistanceFieldFontRenderer::prepareQuadStrip(FontAnnotation* annotation) const {
	auto quadStrip = new QuadStrip(m_texture, m_globalMatricesBindingIndex, true);

	// create quads for each glyph
	for (unsigned i = 0; i < annotation->getText().length(); ++i) {
		quadStrip->addQuad(	m_glyphConfig->getGlyphConfigForCharcode(annotation->getText().at(i))._ll,
							m_glyphConfig->getGlyphConfigForCharcode(annotation->getText().at(i))._advance);
	}
	m_drawingPrimitive = quadStrip;
	setupStylings(annotation);
	annotation->setDirty(false);
}

void glannotations::DistanceFieldFontRenderer::prepareSpline(FontAnnotation* annotation) const {
	auto bendedQuadStrip = new SplineQuads(m_texture, m_globalMatricesBindingIndex, true);

	//get full text length
	float textLength = 0.f;
	for (unsigned i = 0; i < annotation->getText().length(); ++i) {
		textLength += m_glyphConfig->getGlyphConfigForCharcode(annotation->getText().at(i))._advance.x;
	}

	float currentT = 0.f;
	float currentNextT = 0.f;
	for (unsigned i = 0; i < annotation->getText().length(); ++i) {

		currentNextT += m_glyphConfig->getGlyphConfigForCharcode(annotation->getText().at(i))._advance.x / textLength;

		currentT = glm::clamp(currentT, 0.f, 1.f);
		currentNextT = glm::clamp(currentNextT, 0.f, 1.f);
		glm::vec3 secantVec = (annotation->getRenderState()->asSplineState()).retrieveSecantVectorAt(currentT, currentNextT);
		glm::vec3 orthoVec = (annotation->getRenderState()->asSplineState()).retrieveConnectingVectorAt(currentT);

		bendedQuadStrip->addQuad(
			m_glyphConfig->getGlyphConfigForCharcode(annotation->getText().at(i))._ll,
			m_glyphConfig->getGlyphConfigForCharcode(annotation->getText().at(i))._advance,
			secantVec,
			orthoVec
			);
		currentT = currentNextT;
	}

	bendedQuadStrip->setTransformationMatrix((annotation->getRenderState()->asSplineState()).getTransformationMatrix());

	m_drawingPrimitive = bendedQuadStrip;
	setupStylings(annotation);
	annotation->setDirty(false);
}

void glannotations::DistanceFieldFontRenderer::directDrawCall() {
	m_drawingPrimitive->directDrawCall();
}
