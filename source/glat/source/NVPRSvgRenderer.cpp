#include <glat/NVPRSvgRenderer.h>
#include <glat/SVGAnnotation.h>
#include <glat/ViewportState.h>
#include <glat/InternalState.h>
#include <glat/Styles/Outline.h>

void glat::NVPRSvgRenderer::draw(glat::AbstractAnnotation* annotation) {
	glat::SVGAnnotation* currentAnnotation = reinterpret_cast<glat::SVGAnnotation*>(annotation);
	if (annotation->isDirty()) {
		m_height = currentAnnotation->getHeight();
		m_width = currentAnnotation->getWidth();
		clearStencilBuffer();
		initializeSVG(currentAnnotation->getPathString().c_str());
		setupOutline(m_pathBase, annotation->getState()->getStyling("Outline"));
	}

	glat::NVPRRenderer::draw(annotation);
}

void glat::NVPRSvgRenderer::initializeSVG(const char* pathString) {
	glDeletePathsNV(m_pathBase, 1);
	m_pathBase = glGenPathsNV(1);
	glPathStringNV(m_pathBase, GL_PATH_FORMAT_SVG_NV, (GLsizei)strlen(pathString), pathString);
	glPathParameteriNV(m_pathBase, GL_PATH_JOIN_STYLE_NV, GL_ROUND_NV);
}

void glat::NVPRSvgRenderer::drawSetupState(const glat::ViewportState& state) const {
	glDisable(GL_DEPTH_TEST);
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	setupOrthoProjection(state.getLL(), state.getUR(), m_width, m_height);
	
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	drawPath();
	state.setDirty(false);

	// cleanup
	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
}

void glat::NVPRSvgRenderer::drawSetupState(const glat::InternalState& state) const {
	glEnable(GL_DEPTH_TEST);
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	setupInternalProjection(state.getViewProjection(), state.getLL());

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	setupInternalModelview(state.getLL(), state.getLR(), state.getUR(), m_width, m_height);

	drawPath();
	state.setDirty(false);

	// cleanup
	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
}

void glat::NVPRSvgRenderer::drawPath() const {
	glStencilFillPathNV(m_pathBase, GL_COUNT_UP_NV, 0x1);
	glColor3f(0.4f, 0.6f, 1.0f); // green
	glCoverFillPathNV(m_pathBase, GL_BOUNDING_BOX_NV);

	//outline
	if (m_drawOutline) {
		glStencilStrokePathNV(m_pathBase, 0x1, ~0);
		glColor3f(0.2f, 0.2f, 0.2f); // yellow
		glCoverStrokePathNV(m_pathBase, GL_CONVEX_HULL_NV);
	}
}

void glat::NVPRSvgRenderer::drawSetupState(const glat::ExternalBoxState& state) const {
	glEnable(GL_DEPTH_TEST);
	// TODO
	throw std::logic_error("The method or operation is not implemented.");
}

void glat::NVPRSvgRenderer::drawSetupState(const glat::ExternalLabelState& state) const {
	glEnable(GL_DEPTH_TEST);
	// TODO
	throw std::logic_error("The method or operation is not implemented.");
}

void glat::NVPRSvgRenderer::drawSetupState(const glat::InternalPathState& state) const {
	throw std::logic_error("The method or operation is not implemented.");
}
