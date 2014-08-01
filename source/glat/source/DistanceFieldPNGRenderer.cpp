#include <glat/DistanceFieldPNGRenderer.h>
#include <glat/ViewportState.h>
#include <glat/InternalState.h>
#include <glat/PNGAnnotation.h>
#include <glat/TextureManager.h>
#include <glat/QuadStrip.h>

void glat::DistanceFieldPNGRenderer::draw(const glow::ref_ptr<glat::AbstractAnnotation>& annotation) {
	PNGAnnotation* currentAnnotation = dynamic_cast<PNGAnnotation*>(annotation.get());

	if (currentAnnotation->isDirty()) {
		auto quadstrip = new QuadStrip(
			glat::TextureManager::getInstance().getTexture(currentAnnotation->getFileName()),
			m_globalMatricesBindingIndex,
			currentAnnotation->isDistanceField()
		);
		quadstrip->addQuad(glat::QuadStrip::texVec2_t(0.f, 0.f), glat::QuadStrip::texVec2_t(1.f, 1.f));
		m_drawingPrimitive = quadstrip;
		// TODO: avoid importing source image twice
		annotation->getRenderState()->setSourceDimensions(glat::TextureManager::getInstance().getTextureSize(currentAnnotation->getFileName()), m_globalMatricesBindingIndex);
		if (currentAnnotation->isDistanceField()) {
			m_drawingPrimitive->setColor(currentAnnotation->getColor());
			setupOutline(annotation->getRenderState()->getStyling("Outline"));
			setupBumpMap(annotation->getRenderState()->getStyling("BumpMap"));
		}
		annotation->setDirty(false);
	}
	gl::glEnable(gl::GL_BLEND);
	gl::glBlendFunc(gl::GL_SRC_ALPHA, gl::GL_ONE_MINUS_SRC_ALPHA);

	annotation->getRenderState()->draw(*this);

	gl::glDisable(gl::GL_BLEND);
}

glat::DistanceFieldPNGRenderer::DistanceFieldPNGRenderer(gl::GLuint globalMatricesBindingIndex) : AbstractPrimitiveRenderer(globalMatricesBindingIndex) {

}
