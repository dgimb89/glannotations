#include <glat/DistanceFieldPNGRenderer.h>
#include <glat/ViewportState.h>
#include <glat/InternalState.h>
#include <glat/PNGAnnotation.h>
#include <glat/TextureManager.h>
#include <glat/QuadStrip.h>

void glat::DistanceFieldPNGRenderer::draw(const glow::ref_ptr<glat::AbstractAnnotation>& annotation) {
	PNGAnnotation* currentAnnotation = dynamic_cast<PNGAnnotation*>(annotation.get());

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	if (currentAnnotation->isDirty()) {
		auto quadstrip = new QuadStrip(
			glat::TextureManager::getInstance()->getTexture(currentAnnotation->getFileName()),
			currentAnnotation->isDistanceField()
		);
		quadstrip->addQuad(glat::QuadStrip::texVec2_t(0.f, 0.f), glat::QuadStrip::texVec2_t(1.f, 1.f));
		m_drawingPrimitive = quadstrip;

		if (currentAnnotation->isDistanceField()) {
			m_drawingPrimitive->setColor(currentAnnotation->getColor());
			setupOutline(annotation->getRenderState()->getStyling("Outline"));
			setupBumpMap(annotation->getRenderState()->getStyling("BumpMap"));
		}
	}

	annotation->getRenderState()->draw(*this);

	glDisable(GL_BLEND);
}
