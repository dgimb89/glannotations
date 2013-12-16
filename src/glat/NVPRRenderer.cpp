#include <glat/NVPRRenderer.h>
#include <glat/AbstractAnnotation.h>

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
