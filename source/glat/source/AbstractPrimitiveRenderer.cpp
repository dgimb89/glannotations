#include <glbinding/gl/bitfield.h>
#include <glbinding/gl/boolean.h>

#include <glat/AbstractPrimitiveRenderer.h>
#include <glat/Styles/Outline.h>
#include <glat/Styles/BumpMap.h>
#include <glat/ViewportState.h>
#include <glat/InternalState.h>
#include <glat/PathState.h>

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
	if (state.isDirty()) {
		m_drawingPrimitive->setViewportPosition(state.getLL(), state.getLR(), state.getUR());
		state.setDirty(false);
	}

	gl::glPushAttrib(gl::GL_DEPTH_BUFFER_BIT);
	gl::glDisable(gl::GL_DEPTH_TEST);
	gl::glDepthMask(gl::GL_TRUE);
	m_drawingPrimitive->draw();
	gl::glPopAttrib();
}


void glat::AbstractPrimitiveRenderer::drawSetupState(const InternalState& state) const {
	if (state.isDirty()) {
		m_drawingPrimitive->setPosition(state.getLL(), state.getLR(), state.getUR());
		state.setDirty(false);
	}
	m_drawingPrimitive->draw();
}

void glat::AbstractPrimitiveRenderer::drawSetupState(const glat::PathState& state) const {
	throw std::logic_error("The method or operation is not implemented.");
}

glat::AbstractPrimitiveRenderer::AbstractPrimitiveRenderer(gl::GLuint globalMatricesBindingIndex) : AbstractRenderer(globalMatricesBindingIndex) {

}
