#include <glat/DistanceFieldPNGRenderer.h>
#include <glat/ViewportState.h>
#include <glat/InternalState.h>
#include <glat/PNGAnnotation.h>
#include <glat/TextureManager.h>
#include <glat/Quad.h>

void glat::DistanceFieldPNGRenderer::draw(AbstractAnnotation* annotation) {
	PNGAnnotation* currentAnnotation = dynamic_cast<PNGAnnotation*>(annotation);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	if (currentAnnotation->isDirty()) {
		m_drawingPrimitive = new Quad(
			glat::TextureManager::getInstance()->getTexture(currentAnnotation->getFileName()), 
			currentAnnotation->isDistanceField()
		);

		if (currentAnnotation->isDistanceField()) {
			m_drawingPrimitive->setTextColor(currentAnnotation->getColor());
			setupOutline(annotation->getState()->getStyling("Outline"));
			setupBumpMap(annotation->getState()->getStyling("BumpMap"));
		}
	}

	annotation->getState()->draw(*this);

	glDisable(GL_BLEND);
}
