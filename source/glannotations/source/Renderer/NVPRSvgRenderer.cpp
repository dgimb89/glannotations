#include <glbinding/gl/functions.h>
#include <glbinding/gl/enum.h>
#include <glbinding/gl/bitfield.h>

#include <glannotations/Renderer/NVPRSvgRenderer.h>
#include <glannotations/SVGAnnotation.h>
#include <glannotations/States/ViewportState.h>
#include <glannotations/States/SplineState.h>
#include <glannotations/States/QuadState.h>
#include <glannotations/Styles/Outline.h>

#include <string.h>

void glannotations::NVPRSvgRenderer::draw(const globjects::ref_ptr<glannotations::AbstractAnnotation>& annotation) {
	if (annotation->isDirty()) {
		prepare(annotation);
	}

	glannotations::NVPRRenderer::draw(annotation);
}

void glannotations::NVPRSvgRenderer::initializeSVG(const char* pathString) {
	gl::glDeletePathsNV(m_pathBase, 1);
	m_pathBase = gl::glGenPathsNV(1);
	gl::glPathStringNV(m_pathBase, gl::GL_PATH_FORMAT_SVG_NV, (gl::GLsizei)strlen(pathString), pathString);
	gl::glPathParameteriNV(m_pathBase, gl::GL_PATH_JOIN_STYLE_NV, (gl::GLint)gl::GL_ROUND_NV);
}

void glannotations::NVPRSvgRenderer::drawSetupState(glannotations::ViewportState& state) const {
	gl::glDisable(gl::GL_DEPTH_TEST);

	setupOrthoProjection(state.getLL(), state.getUR(), m_width, m_height);
	pushEmptyModelViewMatrix();

	drawPath();
	state.setDirty(false);

	// cleanup
	gl::glEnable(gl::GL_DEPTH_TEST);
	gl::glPopAttrib();
}

void glannotations::NVPRSvgRenderer::drawSetupState(glannotations::QuadState& state) const {
	setupProjection(glannotations::getProjection(getMatricesBindingIndex()));
	setupModelView(glannotations::getView(getMatricesBindingIndex()), state, m_width, m_height);

	drawPath();
	state.setDirty(false);

	// cleanup
	cleanMatrixStacks();
}

void glannotations::NVPRSvgRenderer::drawPath() const {
	gl::glPathCoverDepthFuncNV(gl::GL_ALWAYS);
	gl::glStencilFillPathNV(m_pathBase, gl::GL_COUNT_UP_NV, 0x1);
	gl::glColor3f(0.4f, 0.6f, 1.0f); // green
	gl::glCoverFillPathNV(m_pathBase, gl::GL_BOUNDING_BOX_NV);

	//outline
	if (m_drawOutline) {
		gl::glStencilStrokePathNV(m_pathBase, 0x1, ~0);
		gl::glColor3f(0.2f, 0.2f, 0.2f); // yellow
		gl::glCoverStrokePathNV(m_pathBase, gl::GL_CONVEX_HULL_NV);
	}
}

void glannotations::NVPRSvgRenderer::drawSetupState(glannotations::SplineState& state) const {
	throw std::logic_error("The method or operation is not implemented.");
}

glannotations::NVPRSvgRenderer::NVPRSvgRenderer(gl::GLuint globalMatricesBindingIndex) : NVPRRenderer(globalMatricesBindingIndex) {

}

void glannotations::NVPRSvgRenderer::prepare(const globjects::ref_ptr<glannotations::AbstractAnnotation>& annotation) {
	glannotations::SVGAnnotation* currentAnnotation = reinterpret_cast<glannotations::SVGAnnotation*>(annotation.get());
	m_height = static_cast<float>(currentAnnotation->getHeight());
	m_width = static_cast<float>(currentAnnotation->getWidth());
	clearStencilBuffer();
	initializeSVG(currentAnnotation->getPathString().c_str());
	setupOutline(m_pathBase, annotation->getRenderState()->getStyling("Outline"));
	annotation->setDirty(false);
}
