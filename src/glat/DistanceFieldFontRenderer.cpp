#include <glat/DistanceFieldFontRenderer.h>
#include <glat/ViewportState.h>
#include <glat/InternalState.h>
#include <glat/FontAnnotation.h>
#include <glat/TextureManager.h>
#include <glat/QuadStrip.h>

void glat::DistanceFieldFontRenderer::draw(AbstractAnnotation* annotation) {
	FontAnnotation* currentAnnotation = dynamic_cast<FontAnnotation*>(annotation);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	if (currentAnnotation->isDirty()) {
		m_glyphConfig = new glat::GlyphSetConfig(currentAnnotation->getFontName());		
		auto quadStrip = new QuadStrip(glat::TextureManager::getInstance()->getTexture(m_glyphConfig->getGlyphsetImageName()));
		for (unsigned i = 0; i < currentAnnotation->getText().length(); ++i) {
			quadStrip->addQuad(	m_glyphConfig->getGlyphConfigForCharcode(currentAnnotation->getText().at(i)).ll, 
								m_glyphConfig->getGlyphConfigForCharcode(currentAnnotation->getText().at(i)).ur);
		}
		m_drawingPrimitive = quadStrip;

		m_drawingPrimitive->setTextColor(currentAnnotation->getColor());
		setupOutline(annotation->getState()->getStyling("Outline"));
		setupBumpMap(annotation->getState()->getStyling("BumpMap"));
	}

	annotation->getState()->draw(*this);

	glDisable(GL_BLEND);
}