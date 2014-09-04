#include <glbinding/gl/bitfield.h>

#include <glat/DistanceFieldFontRenderer.h>
#include <glat/ViewportState.h>
#include <glat/InternalState.h>
#include <glat/FontAnnotation.h>
#include <glat/TextureManager.h>
#include <glat/QuadStrip.h>
#include <glat/GlyphSetConfig.h>

void glat::DistanceFieldFontRenderer::draw(const glo::ref_ptr<glat::AbstractAnnotation>& annotation) {
	FontAnnotation* currentAnnotation = dynamic_cast<FontAnnotation*>(annotation.get());
	gl::glPushAttrib(gl::GL_COLOR_BUFFER_BIT);

	gl::glEnable(gl::GL_BLEND);
	gl::glBlendFunc(gl::GL_SRC_ALPHA, gl::GL_ONE_MINUS_SRC_ALPHA);

	if (currentAnnotation->isDirty()) {
		setupGlyphQuadstrip(currentAnnotation);
		m_drawingPrimitive->setColor(currentAnnotation->getColor());
		setupOutline(annotation->getRenderState()->getStyling("Outline"));
		setupBumpMap(annotation->getRenderState()->getStyling("BumpMap"));
		annotation->setDirty(false);
	}

	annotation->getRenderState()->draw(*this);

	gl::glPopAttrib();
}

glat::DistanceFieldFontRenderer::DistanceFieldFontRenderer(gl::GLuint matricesBindingIndex) : AbstractPrimitiveRenderer(matricesBindingIndex) {

}

void glat::DistanceFieldFontRenderer::setupGlyphQuadstrip(glat::FontAnnotation* annotation) {
	glat::GlyphSetConfig glyphConfig(annotation->getFontName());
	auto quadStrip = new QuadStrip(glat::TextureManager::getInstance().getTexture(glyphConfig.getGlyphsetImageName()), m_globalMatricesBindingIndex, true);
	for (unsigned i = 0; i < annotation->getText().length(); ++i) {
		quadStrip->addQuad(glyphConfig.getGlyphConfigForCharcode(annotation->getText().at(i))._ll,
			glyphConfig.getGlyphConfigForCharcode(annotation->getText().at(i))._advance);
	}
	float pixelHeight = annotation->getFontSize() * quadStrip->getQuadstripRowCount() * 4.f / 3.f; // 1 zoll = 72 pt = 96 px
	annotation->getRenderState()->setSourceDimensions(static_cast<unsigned short>((quadStrip->getQuadStripWidth() / quadStrip->getQuadStripHeight()) * pixelHeight), static_cast<unsigned short>(pixelHeight), m_globalMatricesBindingIndex);
	m_drawingPrimitive = quadStrip;
}
