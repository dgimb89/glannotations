#define __STDC_LIMIT_MACROS

#include <glbinding/gl/bitfield.h>
#include <glbinding/gl/functions.h>

#include <glannotations/Renderer/DistanceFieldFontRenderer.h>
#include <glannotations/States/ViewportState.h>
#include <glannotations/States/QuadState.h>
#include <glannotations/FontAnnotation.h>
#include <glannotations/Common/TextureManager.h>
#include <glannotations/Renderer/QuadStrip.h>
#include <glannotations/Renderer/BendedQuadStrip.h> //todo:anne optimize includes?
#include <glannotations/Utility/GlyphSetConfig.h>

void glannotations::DistanceFieldFontRenderer::draw(const globjects::ref_ptr<glannotations::AbstractAnnotation>& annotation) {
	if (annotation->isDirty()) {
		prepare(annotation);
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
	// TODO: just save the text and font name here and defer primitive creation until we know the state
	// take a look at @AbstractPrimitiveRenderer::drawSetupState --> create primitive when state is dirty
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
}

void glannotations::DistanceFieldFontRenderer::prepare(const globjects::ref_ptr<glannotations::AbstractAnnotation>& annotation) {
	//todo:anne vorbereiten? BendendQuadStrip?
	FontAnnotation* currentAnnotation = dynamic_cast<FontAnnotation*>(annotation.get());
	setupGlyphQuadstrip(currentAnnotation);
	m_drawingPrimitive->setColor(currentAnnotation->getColor());
	setupOutline(annotation->getRenderState()->getStyling("Outline"));
	setupBumpMap(annotation->getRenderState()->getStyling("BumpMap"));
	annotation->setDirty(false);
}
