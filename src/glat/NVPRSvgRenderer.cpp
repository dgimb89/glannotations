#include <glat/NVPRSvgRenderer.h>
#include <glat/SVGAnnotation.h>
#include <glat/ViewportState.h>
#include <glat/Outline.h>

void glat::NVPRSvgRenderer::draw(glat::AbstractAnnotation* annotation) {
	glat::SVGAnnotation* currentAnnotation = reinterpret_cast<glat::SVGAnnotation*>(annotation);
	if (annotation->isDirty()) {
		glPathStringNV(m_pathObj, GL_PATH_FORMAT_SVG_NV, (GLsizei)strlen(currentAnnotation->getPathString().c_str()), currentAnnotation->getPathString().c_str());
		glPathParameteriNV(m_pathObj, GL_PATH_JOIN_STYLE_NV, GL_ROUND_NV);
		glPathParameterfNV(m_pathObj, GL_PATH_STROKE_WIDTH_NV, 2.f);
	}
	glat::NVPRRenderer::draw(annotation);
}

void glat::NVPRSvgRenderer::drawSetupState(const glat::ViewportState& state) const {
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();

	glOrtho(0, 600, 0, 400, -1, 1);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	glStencilFillPathNV(m_pathObj, GL_COUNT_UP_NV, 0x1);
	glStencilFunc(GL_NOTEQUAL, 0, 0x1);
	glStencilOp(GL_KEEP, GL_KEEP, GL_ZERO);
	glColor3f(0.75f, 0.75f, 0.75f); // green
	glCoverFillPathNV(m_pathObj, GL_BOUNDING_BOX_NV);

	//outline
	glStencilStrokePathNV(m_pathObj, 0x1, ~0);
	glColor3f(0.2f, 0.2f, 0.2f); // yellow
	glCoverStrokePathNV(m_pathObj, GL_CONVEX_HULL_NV);

	// cleanup
	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
}
