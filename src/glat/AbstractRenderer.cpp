#include <glat/AbstractRenderer.h>
#define NOMINMAX
#include <Windows.h>
#include <gl/GL.h>

void glat::AbstractRenderer::setupViewportRendering() {
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();

	int viewportParams[4];
	glGetIntegerv(GL_VIEWPORT, viewportParams);
	glOrtho(viewportParams[0], viewportParams[2], viewportParams[3], viewportParams[1], -1.0, 10.0);
	glMatrixMode(GL_MODELVIEW);

	glLoadIdentity();
	glDisable(GL_CULL_FACE);
	glDepthMask(GL_FALSE);
	glDisable(GL_DEPTH_TEST);
}