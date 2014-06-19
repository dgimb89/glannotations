#include <glat/DistanceFieldFontRenderer.h>
#include <glat/ViewportState.h>
#include <glat/InternalState.h>
#include <glat/FontAnnotation.h>
#include <glat/TextureManager.h>
#include <glat/QuadStrip.h>

void glat::DistanceFieldFontRenderer::draw(const glow::ref_ptr<glat::AbstractAnnotation>& annotation) {
	FontAnnotation* currentAnnotation = dynamic_cast<FontAnnotation*>(annotation.get());
	gl::glEnable(gl::GL_BLEND);
	gl::glBlendFunc(gl::GL_SRC_ALPHA, gl::GL_ONE_MINUS_SRC_ALPHA);

	if (currentAnnotation->isDirty()) {
		m_glyphConfig = new glat::GlyphSetConfig(currentAnnotation->getFontName());		
		auto quadStrip = new QuadStrip(glat::TextureManager::getInstance().getTexture(m_glyphConfig->getGlyphsetImageName()));
		for (unsigned i = 0; i < currentAnnotation->getText().length(); ++i) {
			quadStrip->addQuad(	m_glyphConfig->getGlyphConfigForCharcode(currentAnnotation->getText().at(i))._ll, 
								m_glyphConfig->getGlyphConfigForCharcode(currentAnnotation->getText().at(i))._advance);
		}
		m_drawingPrimitive = quadStrip;

		m_drawingPrimitive->setColor(currentAnnotation->getColor());
		setupOutline(annotation->getRenderState()->getStyling("Outline"));
		setupBumpMap(annotation->getRenderState()->getStyling("BumpMap"));
		annotation->setDirty(false);
	}

	annotation->getRenderState()->draw(*this);
	gl::glDisable(gl::GL_BLEND);
}