#include <glbinding/gl/functions.h>

#include <glat/NVPRSvgRenderer.h>
#include <glat/SVGAnnotation.h>
#include <glat/ViewportState.h>
#include <glat/InternalState.h>
#include <glat/Styles/Outline.h>

void glat::NVPRSvgRenderer::draw(const glow::ref_ptr<glat::AbstractAnnotation>& annotation) {
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
	gl::glDisable(gl::GL_DEPTH_TEST);
	gl::glMatrixMode(gl::GL_PROJECTION);
	gl::glPushMatrix();
	gl::glLoadIdentity();
	setupOrthoProjection(state.getLL(), state.getUR(), m_width, m_height);
	
	gl::glMatrixMode(gl::GL_MODELVIEW);
	gl::glPushMatrix();
	gl::glLoadIdentity();

	drawPath();
	state.setDirty(false);

	// cleanup
	gl::glPopMatrix();
	gl::glMatrixMode(gl::GL_PROJECTION);
	gl::glPopMatrix();
}

void glat::NVPRSvgRenderer::drawSetupState(const glat::InternalState& state) const {
	gl::glEnable(gl::GL_DEPTH_TEST);
	gl::glMatrixMode(gl::GL_PROJECTION);
	gl::glPushMatrix();
	gl::glLoadIdentity();
	setupInternalProjection(state.getViewProjection(), state.getLL());

	gl::glMatrixMode(gl::GL_MODELVIEW);
	gl::glPushMatrix();
	gl::glLoadIdentity();
	setupInternalModelview(state.getLL(), state.getLR(), state.getUR(), m_width, m_height);

	drawPath();
	state.setDirty(false);

	// cleanup
	gl::glPopMatrix();
	gl::glMatrixMode(gl::GL_PROJECTION);
	gl::glPopMatrix();
}

void glat::NVPRSvgRenderer::drawPath() const {
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
