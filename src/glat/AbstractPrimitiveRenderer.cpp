#include <glat/AbstractPrimitiveRenderer.h>

#include <glat/Styles/Outline.h>
#include <glat/Styles/BumpMap.h>
#include <glat/ViewportState.h>
#include <glat/InternalState.h>
#include <glat/ExternalBoxState.h>

void glat::AbstractPrimitiveRenderer::setupOutline(const Styling* outline) {
	if (outline == nullptr) return;
	const Styles::Outline* outlineStyle = reinterpret_cast<const Styles::Outline*>(outline);
	m_drawingPrimitive->setOutline(outlineStyle->getWidth()*0.1f, outlineStyle->getColor());
}


void glat::AbstractPrimitiveRenderer::setupBumpMap(const Styling* bumpMap) {
	if (bumpMap == nullptr) return;
	const Styles::BumpMap* bumpMapStyle = reinterpret_cast<const Styles::BumpMap*>(bumpMap);
	m_drawingPrimitive->setBumpMap(bumpMapStyle->getIntensity());
}

void glat::AbstractPrimitiveRenderer::drawSetupState(const ViewportState& state) const {
	glDisable(GL_DEPTH_TEST);
	if (state.isDirty()) {
		m_drawingPrimitive->setPosition(glm::vec3(state.getLL(), 0.0), glm::vec3(state.getLR(), 0.0), glm::vec3(state.getUR(), 0.0));
		state.setDirty(false);
	}
	m_drawingPrimitive->draw();
	glEnable(GL_DEPTH_TEST);
}


void glat::AbstractPrimitiveRenderer::drawSetupState(const InternalState& state) const {
	if (state.isDirty()) {
		m_drawingPrimitive->setPosition(state.getLL(), state.getLR(), state.getUR(), state.getViewProjection());
		state.setDirty(false);
	}
	m_drawingPrimitive->draw();
}

void glat::AbstractPrimitiveRenderer::drawSetupState(const glat::ExternalBoxState& state) const {
	if (state.isDirty()) {
		state.updateInternalPosition();
		m_drawingPrimitive->setPosition(state.getLL(), state.getLR(), state.getUR(), state.getViewProjection());
		state.setupExternalPrimitives();
		state.setDirty(false);
	}
	state.drawExternalPrimitives();
	m_drawingPrimitive->draw();
}
