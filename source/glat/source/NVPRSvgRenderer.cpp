#include <glbinding/gl/functions.h>
#include <glbinding/gl/enum.h>
#include <glbinding/gl/bitfield.h>

#include <glat/NVPRSvgRenderer.h>
#include <glat/SVGAnnotation.h>
#include <glat/ViewportState.h>
#include <glat/InternalState.h>
#include <glat/Styles/Outline.h>

#include <string.h>

void glat::NVPRSvgRenderer::draw(const glo::ref_ptr<glat::AbstractAnnotation>& annotation) {
	glat::SVGAnnotation* currentAnnotation = reinterpret_cast<glat::SVGAnnotation*>(annotation.get());
	if (annotation->isDirty()) {
		m_height = currentAnnotation->getHeight();
		m_width = currentAnnotation->getWidth();
		clearStencilBuffer();
		initializeSVG(currentAnnotation->getPathString().c_str());
		setupOutline(m_pathBase, annotation->getRenderState()->getStyling("Outline"));
		annotation->setDirty(false);
	}

	glat::NVPRRenderer::draw(annotation);
}

void glat::NVPRSvgRenderer::initializeSVG(const char* pathString) {
	gl::glDeletePathsNV(m_pathBase, 1);
	m_pathBase = gl::glGenPathsNV(1);
	gl::glPathStringNV(m_pathBase, gl::GL_PATH_FORMAT_SVG_NV, (gl::GLsizei)strlen(pathString), pathString);
	gl::glPathParameteriNV(m_pathBase, gl::GL_PATH_JOIN_STYLE_NV, (gl::GLint)gl::GL_ROUND_NV);
}

void glat::NVPRSvgRenderer::drawSetupState(const glat::ViewportState& state) const {
	gl::glPushAttrib(gl::GL_DEPTH_BUFFER_BIT);
	gl::glDisable(gl::GL_DEPTH_TEST);

	setupOrthoProjection(state.getLL(), state.getUR(), m_width, m_height);
	pushEmptyModelViewMatrix();

	drawPath();
	state.setDirty(false);

	// cleanup
	cleanMatrixStacks();
	gl::glPopAttrib();
}

void glat::NVPRSvgRenderer::drawSetupState(const glat::InternalState& state) const {
	setupProjection(glat::getProjection(getMatricesBindingIndex()));
	setupModelView(glat::getView(getMatricesBindingIndex()), state, m_width, m_height);

	drawPath();
	state.setDirty(false);

	// cleanup
	cleanMatrixStacks();
}

void glat::NVPRSvgRenderer::drawPath() const {
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

void glat::NVPRSvgRenderer::drawSetupState(const glat::PathState& state) const {
	throw std::logic_error("The method or operation is not implemented.");
}

glat::NVPRSvgRenderer::NVPRSvgRenderer(gl::GLuint globalMatricesBindingIndex) : NVPRRenderer(globalMatricesBindingIndex) {

}
