#include <glat/DistanceFieldFontRenderer.h>
#include <glat/ViewportState.h>
#include <glat/InternalState.h>
#include <glat/FontAnnotation.h>
#include <glat/TextureManager.h>

void glat::DistanceFieldFontRenderer::draw(AbstractAnnotation* annotation) {
	FontAnnotation* currentAnnotation = dynamic_cast<FontAnnotation*>(annotation);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	if (currentAnnotation->isDirty()) {
		m_glyphConfig = new glat::GlyphSetConfig(currentAnnotation->getFontName());
		std::vector<char> charCodes;
		for (unsigned i = 0; i < currentAnnotation->getText().length(); ++i) {
			charCodes.push_back(currentAnnotation->getText()[i]);
		}

		// todo: quadstrip
		//glyphSetConfig.getGlyphConfigForCharcode(charCodes.back());
		//m_quad = new Quad(glat::TextureManager::getInstance()->getTexture(std::string(path)));

		setupOutline(annotation->getState()->getStyling("Outline"));
		setupBumpMap(annotation->getState()->getStyling("BumpMap"));
	}

	m_drawingPrimitive->setTextColor(currentAnnotation->getColor());
	annotation->getState()->draw(*this);

	glDisable(GL_BLEND);
}

void glat::DistanceFieldFontRenderer::drawSetupState(const ViewportState& state) const {
	glDisable(GL_DEPTH_TEST);
	if (state.isDirty()) {
		m_drawingPrimitive->setPosition(glm::vec3(state.getLL(), 0.0), glm::vec3(state.getLR(), 0.0), glm::vec3(state.getUR(), 0.0));
		state.setDirty(false);
	}
	m_drawingPrimitive->draw();
	glEnable(GL_DEPTH_TEST);
}


void glat::DistanceFieldFontRenderer::drawSetupState(const InternalState& state) const {
	if (state.isDirty()) {
		m_drawingPrimitive->setPosition(state.getLL(), state.getLR(), state.getUR(), state.getViewProjection());
		state.setDirty(false);
	}
	m_drawingPrimitive->draw();
}
