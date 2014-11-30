#include <glbinding/gl/bitfield.h>
#include <glbinding/gl/boolean.h>
#include <glbinding/gl/functions.h>
#include <glbinding/gl/enum.h>

#include <glannotations/AbstractPrimitiveRenderer.h>
#include <glannotations/Styles/Outline.h>
#include <glannotations/Styles/BumpMap.h>
#include <glannotations/ViewportState.h>
#include <glannotations/InternalState.h>
#include <glannotations/PathState.h>

void glannotations::AbstractPrimitiveRenderer::setupOutline(const Styling* outline) {
	if (outline == nullptr) return;
	const Styles::Outline* outlineStyle = reinterpret_cast<const Styles::Outline*>(outline);
	m_drawingPrimitive->setOutline(outlineStyle->getWidth()*0.1f, outlineStyle->getColor());
}


void glannotations::AbstractPrimitiveRenderer::setupBumpMap(const Styling* bumpMap) {
	if (bumpMap == nullptr) return;
	const Styles::BumpMap* bumpMapStyle = reinterpret_cast<const Styles::BumpMap*>(bumpMap);
	m_drawingPrimitive->setBumpMap(bumpMapStyle->getIntensity());
}

void glannotations::AbstractPrimitiveRenderer::drawSetupState(const ViewportState& state) const {
	if (state.isDirty()) {
		m_drawingPrimitive->setViewportPosition(state.getLL(), state.getLR(), state.getUR());
		state.setDirty(false);
	}

	gl::glDisable(gl::GL_DEPTH_TEST);
	gl::glDepthMask(gl::GL_TRUE);
	m_drawingPrimitive->draw();
	gl::glEnable(gl::GL_DEPTH_TEST);
}


void glannotations::AbstractPrimitiveRenderer::drawSetupState(const InternalState& state) const {
	if (state.isDirty()) {
		m_drawingPrimitive->setPosition(state.getLL(), state.getLR(), state.getUR());
		state.setDirty(false);
	}
	m_drawingPrimitive->draw();
}

void glannotations::AbstractPrimitiveRenderer::drawSetupState(const glannotations::PathState& state) const {
	throw std::logic_error("The method or operation is not implemented.");
}

glannotations::AbstractPrimitiveRenderer::AbstractPrimitiveRenderer(gl::GLuint globalMatricesBindingIndex) : AbstractRenderer(globalMatricesBindingIndex) {

}
