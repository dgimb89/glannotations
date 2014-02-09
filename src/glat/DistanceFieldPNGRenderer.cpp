#include <glat/DistanceFieldPNGRenderer.h>
#include <glat/ViewportState.h>
#include <glat/InternalState.h>
#include <glat/PNGAnnotation.h>
#include <glat/TextureManager.h>

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

void glat::DistanceFieldPNGRenderer::drawSetupState(const ViewportState& state) const {
	glDisable(GL_DEPTH_TEST);
	if (state.isDirty()) {
		m_drawingPrimitive->setPosition(glm::vec3(state.getLL(), 0.0), glm::vec3(state.getLR(), 0.0), glm::vec3(state.getUR(), 0.0));
		state.setDirty(false);
	}
	m_drawingPrimitive->draw();
	glEnable(GL_DEPTH_TEST);
}


void glat::DistanceFieldPNGRenderer::drawSetupState(const InternalState& state) const {
	if (state.isDirty()) {
		m_drawingPrimitive->setPosition(state.getLL(), state.getLR(), state.getUR(), state.getViewProjection());
		state.setDirty(false);
	}
	m_drawingPrimitive->draw();
}
