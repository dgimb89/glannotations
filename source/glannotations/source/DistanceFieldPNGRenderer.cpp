#include <glbinding/gl/bitfield.h>
#include <glbinding/gl/functions.h>

#include <glannotations/DistanceFieldPNGRenderer.h>
#include <glannotations/ViewportState.h>
#include <glannotations/InternalState.h>
#include <glannotations/PNGAnnotation.h>
#include <glannotations/TextureManager.h>
#include <glannotations/QuadStrip.h>

void glannotations::DistanceFieldPNGRenderer::draw(const globjects::ref_ptr<glannotations::AbstractAnnotation>& annotation) {
	if (annotation->isDirty()) {
		prepare(annotation);
	}

	gl::glEnable(gl::GL_BLEND);
	gl::glBlendFunc(gl::GL_SRC_ALPHA, gl::GL_ONE_MINUS_SRC_ALPHA);
	annotation->getRenderState()->draw(*this);
	gl::glDisable(gl::GL_BLEND);
}

glannotations::DistanceFieldPNGRenderer::DistanceFieldPNGRenderer(gl::GLuint globalMatricesBindingIndex) : AbstractPrimitiveRenderer(globalMatricesBindingIndex) {

}

void glannotations::DistanceFieldPNGRenderer::prepare(const globjects::ref_ptr<glannotations::AbstractAnnotation>& annotation) {
	PNGAnnotation* currentAnnotation = dynamic_cast<PNGAnnotation*>(annotation.get());
	auto quadstrip = new QuadStrip(
		glannotations::TextureManager::getInstance().getTexture(currentAnnotation->getFileName()),
		m_globalMatricesBindingIndex,
		currentAnnotation->isDistanceField()
		);
	quadstrip->addQuad(glannotations::QuadStrip::texVec2_t(0.f, 0.f), glannotations::QuadStrip::texVec2_t(1.f, 1.f));
	m_drawingPrimitive = quadstrip;
	if (currentAnnotation->isDistanceField()) {
		m_drawingPrimitive->setColor(currentAnnotation->getColor());
		setupOutline(annotation->getRenderState()->getStyling("Outline"));
		setupBumpMap(annotation->getRenderState()->getStyling("BumpMap"));
	}
	annotation->setDirty(false);
}
