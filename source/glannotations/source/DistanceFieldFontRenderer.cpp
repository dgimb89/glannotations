#define __STDC_LIMIT_MACROS

#include <glbinding/gl/bitfield.h>
#include <glbinding/gl/functions.h>

#include <glannotations/DistanceFieldFontRenderer.h>
#include <glannotations/ViewportState.h>
#include <glannotations/InternalState.h>
#include <glannotations/FontAnnotation.h>
#include <glannotations/TextureManager.h>
#include <glannotations/QuadStrip.h>
#include <glannotations/BendedQuadStrip.h> //todo:anne optimize includes?
#include <glannotations/GlyphSetConfig.h>

void glannotations::DistanceFieldFontRenderer::draw(const globjects::ref_ptr<glannotations::AbstractAnnotation>& annotation) {
	if (annotation->isDirty()) {
		prepare(annotation);
		//todo:anne vorbereiten? BendendQuadStrip?
	}

	gl::glEnable(gl::GL_BLEND);
	gl::glBlendFunc(gl::GL_SRC_ALPHA, gl::GL_ONE_MINUS_SRC_ALPHA);
	annotation->getRenderState()->draw(*this);
	gl::glDisable(gl::GL_BLEND);
}

glannotations::DistanceFieldFontRenderer::DistanceFieldFontRenderer(gl::GLuint matricesBindingIndex) : AbstractPrimitiveRenderer(matricesBindingIndex) {
}

void glannotations::DistanceFieldFontRenderer::setupGlyphQuadstrip(glannotations::FontAnnotation* annotation) {
	glannotations::GlyphSetConfig glyphConfig(annotation->getFontName());

	try {
		annotation->getRenderState()->asSplineState();
		auto bendedQuadStrip = new BendedQuadStrip(
			glannotations::TextureManager::getInstance().getTexture(glyphConfig.getGlyphsetImageName()),
			m_globalMatricesBindingIndex,
			true
			);

		//get full text length
		float textLength = 0.f;
		for (unsigned i = 0; i < annotation->getText().length(); ++i) {
			textLength += glyphConfig.getGlyphConfigForCharcode(annotation->getText().at(i))._advance.x;
		}

		float currentT = 0.f;
		float currentNextT = 0.f;
		for (unsigned i = 0; i < annotation->getText().length(); ++i) {

			currentNextT += glyphConfig.getGlyphConfigForCharcode(annotation->getText().at(i))._advance.x / textLength;
			glm::vec3 secantVec = (annotation->getRenderState()->asSplineState()).retrieveSecantVectorAt(currentT, currentNextT);
			glm::vec3 orthoVec = (annotation->getRenderState()->asSplineState()).retrieveConnectingVectorAt(currentT);

			bendedQuadStrip->addQuad(
				glyphConfig.getGlyphConfigForCharcode(annotation->getText().at(i))._ll,
				glyphConfig.getGlyphConfigForCharcode(annotation->getText().at(i))._advance,
				secantVec,
				orthoVec
				);
			currentT = currentNextT;
		}

		m_drawingPrimitive = bendedQuadStrip;
	}
	catch (...){ //Katastrophenschutz wegen bad_cast. todo:anne double dispatch oder irgendwas Sinnvolles
		auto quadStrip = new QuadStrip(
			glannotations::TextureManager::getInstance().getTexture(glyphConfig.getGlyphsetImageName()),
			m_globalMatricesBindingIndex,
			true
			);
		for (unsigned i = 0; i < annotation->getText().length(); ++i) {
			quadStrip->addQuad(glyphConfig.getGlyphConfigForCharcode(annotation->getText().at(i))._ll,
				glyphConfig.getGlyphConfigForCharcode(annotation->getText().at(i))._advance);
		}
		m_drawingPrimitive = quadStrip;
	}

	//float pixelHeight = annotation->getFontSize() * quadStrip->getQuadstripRowCount() * 4.f / 3.f; // 1 zoll = 72 pt = 96 px
}

void glannotations::DistanceFieldFontRenderer::prepare(const globjects::ref_ptr<glannotations::AbstractAnnotation>& annotation) {
	FontAnnotation* currentAnnotation = dynamic_cast<FontAnnotation*>(annotation.get());
	setupGlyphQuadstrip(currentAnnotation);
	m_drawingPrimitive->setColor(currentAnnotation->getColor());
	setupOutline(annotation->getRenderState()->getStyling("Outline"));
	setupBumpMap(annotation->getRenderState()->getStyling("BumpMap"));
	annotation->setDirty(false);
}
