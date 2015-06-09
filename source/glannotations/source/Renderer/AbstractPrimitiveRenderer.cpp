#include <glbinding/gl/bitfield.h>
#include <glbinding/gl/boolean.h>
#include <glbinding/gl/functions.h>
#include <glbinding/gl/enum.h>

#include <glannotations/Renderer/AbstractPrimitiveRenderer.h>
#include <glannotations/Styles/Outline.h>
#include <glannotations/Styles/BumpMap.h>
#include <glannotations/States/ViewportState.h>
#include <glannotations/States/QuadState.h>
#include <glannotations/States/SplineState.h>
#include <glannotations/Common/globals.h>

void glannotations::AbstractPrimitiveRenderer::setupOutline(const Styling* outline) {
	if (outline == nullptr) return;
	const Styles::Outline* outlineStyle = reinterpret_cast<const Styles::Outline*>(outline);
	m_drawingPrimitive->setOutline(outlineStyle->getWidth(), outlineStyle->getColor());
}


void glannotations::AbstractPrimitiveRenderer::setupBumpMap(const Styling* bumpMap) {
	if (bumpMap == nullptr) return;
	const Styles::BumpMap* bumpMapStyle = reinterpret_cast<const Styles::BumpMap*>(bumpMap);
	m_drawingPrimitive->setBumpMap(bumpMapStyle->getIntensity());
}

void glannotations::AbstractPrimitiveRenderer::drawSetupState(ViewportState& state) const {
	// in order to allow correct constrainting for screen space extends, the state needs to know the current screen aspect ratio
	state.setScreenAspectRatio(glannotations::getAspectRatio(getMatricesBindingIndex()));
	if (state.isDirty()) {
		state.updateExtends(m_drawingPrimitive->getExtends());
		m_drawingPrimitive->setViewportPosition(state.getLL(), state.getLR(), state.getUR());
		state.setDirty(false);
	}

	gl::glDisable(gl::GL_DEPTH_TEST);
	gl::glDepthMask(gl::GL_TRUE);
	m_drawingPrimitive->draw();
	gl::glEnable(gl::GL_DEPTH_TEST);
}


void glannotations::AbstractPrimitiveRenderer::drawSetupState(QuadState& state) const {
	if (state.isDirty()) {
		state.updateExtends(m_drawingPrimitive->getExtends());
		m_drawingPrimitive->setPosition(state.getLL(), state.getLR(), state.getUR());
		state.setDirty(false);
	}
	m_drawingPrimitive->draw();
}

void glannotations::AbstractPrimitiveRenderer::drawSetupState(glannotations::SplineState& state) const {
	if (state.isDirty()) {
		//m_drawingPrimitive is BendedQuadStrip here!
		m_drawingPrimitive->setPosition(state.getLL(), glm::vec3(), glm::vec3());
		state.setDirty(false);
	}
	m_drawingPrimitive->draw();
}

glannotations::AbstractPrimitiveRenderer::AbstractPrimitiveRenderer(gl::GLuint globalMatricesBindingIndex) : AbstractRenderer(globalMatricesBindingIndex) {

}
