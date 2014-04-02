#include <glat/NVPRRenderer.h>
#include <glat/AbstractAnnotation.h>
#include <glat/Styles/Outline.h>

void glat::NVPRRenderer::draw(glat::AbstractAnnotation* annotation) {
	// disable depth test and back face culling
	glDisable(GL_DEPTH_TEST);
	// enable stencil test as needed by nvpr
	glEnable(GL_STENCIL_TEST);

	// double dispatch to draw specific state
	annotation->getState()->draw(*this);

	// disable stencil test when finished
	glDisable(GL_STENCIL_TEST);
	// enable depth test again
	glEnable(GL_DEPTH_TEST);
}

void glat::NVPRRenderer::clearStencilBuffer() {
	glClearStencil(0);
	glStencilMask(~0);
	glClear(GL_STENCIL_BUFFER_BIT);
}

glat::NVPRRenderer::NVPRRenderer() {
	initializeNVPR();
}

void glat::NVPRRenderer::setupOutline(const glat::Styling* outline, float scaleFactor /* = 1.f*/) {
	m_drawOutline = false;
	if (outline == nullptr) return;
	m_drawOutline = true;
	const glat::Styles::Outline* outlineStyle = reinterpret_cast<const glat::Styles::Outline*>(outline);
	glPathParameterfNV(m_pathSettings, GL_PATH_STROKE_WIDTH_NV, scaleFactor * outlineStyle->getWidth());
}

void glat::NVPRRenderer::setupOrthoProjection(glm::vec2 llf, glm::vec2 urb, float width, float height, float yMin /*= 0.f*/) const {
	float screenWidth = urb.x - llf.x;
	float screenHeight = urb.y - llf.y;

	glOrtho((llf.x + 1) / screenWidth * (-width),
		width + (1 - urb.x) / screenWidth * width,
		yMin - height * (llf.y + 1) / screenHeight,
		(yMin + height) + height * (1 - urb.y) / screenHeight,
		-1, 1);
}
