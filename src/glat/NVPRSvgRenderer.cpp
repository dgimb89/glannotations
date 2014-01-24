#include <glat/NVPRSvgRenderer.h>
#include <glat/SVGAnnotation.h>
#include <glat/ViewportState.h>
#include <glat/InternalState.h>
#include <glat/Outline.h>

void glat::NVPRSvgRenderer::draw(glat::AbstractAnnotation* annotation) {
	glat::SVGAnnotation* currentAnnotation = reinterpret_cast<glat::SVGAnnotation*>(annotation);
	if (annotation->isDirty()) {
		glPathStringNV(m_pathSettings, GL_PATH_FORMAT_SVG_NV, (GLsizei)strlen(currentAnnotation->getPathString().c_str()), currentAnnotation->getPathString().c_str());
		glPathParameteriNV(m_pathSettings, GL_PATH_JOIN_STYLE_NV, GL_ROUND_NV);
		setupOutline(annotation->getState()->getStyling("Outline"));
		m_height = currentAnnotation->getHeight();
		m_width = currentAnnotation->getWidth();
		annotation->setDirty(false);
	}
	glat::NVPRRenderer::draw(annotation);
}

void glat::NVPRSvgRenderer::drawSetupState(const glat::ViewportState& state) const {
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	setupOrthoProjection(state.getLLF(), state.getURB(), m_width, m_height);
	
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	glStencilFillPathNV(m_pathSettings, GL_COUNT_UP_NV, 0x1);
	glStencilFunc(GL_NOTEQUAL, 0, 0x1);
	glStencilOp(GL_KEEP, GL_KEEP, GL_ZERO);
	glColor3f(0.4f, 0.6f, 1.0f); // green
	glCoverFillPathNV(m_pathSettings, GL_BOUNDING_BOX_NV);

	//outline
	if (m_drawOutline) {
		glStencilStrokePathNV(m_pathSettings, 0x1, ~0);
		glColor3f(0.2f, 0.2f, 0.2f); // yellow
		glCoverStrokePathNV(m_pathSettings, GL_CONVEX_HULL_NV);
	}

	// cleanup
	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
}

void glat::NVPRSvgRenderer::drawSetupState(const glat::InternalState& state) const {

}