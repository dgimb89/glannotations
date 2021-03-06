#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glbinding/gl/functions.h>
#include <glbinding/gl/bitfield.h>
#include <glbinding/gl/enum.h>

#include <glannotations/Renderer/NVPRRenderer.h>
#include <glannotations/AbstractAnnotation.h>
#include <glannotations/Styles/Outline.h>
#include <glannotations/States/QuadState.h>

void glannotations::NVPRRenderer::draw(const globjects::ref_ptr<glannotations::AbstractAnnotation>& annotation) {
	// enable stencil test as needed by nvpr

	gl::glEnable(gl::GL_STENCIL_TEST);
	gl::glStencilFunc(gl::GL_NOTEQUAL, 0, 0x1F);
	gl::glStencilOp(gl::GL_KEEP, gl::GL_KEEP, gl::GL_ZERO);
	// double dispatch to draw specific state
	annotation->getRenderState()->draw(annotation, *this);

	// disable stencil test when finished
	gl::glDisable(gl::GL_STENCIL_TEST);
}

void glannotations::NVPRRenderer::clearStencilBuffer() {
	// stencil buffer only needs to be cleared when damaged or window resized
	gl::glClearStencil(0);
	gl::glStencilMask(~0);
	gl::glClear(gl::GL_STENCIL_BUFFER_BIT);
}

void glannotations::NVPRRenderer::setupOrthoProjection(glm::vec2 llf, glm::vec2 urb, float width, float height, float yMin /*= 0.f*/) const {
	gl::glMatrixMode(gl::GL_PROJECTION);
	gl::glPushMatrix();
	gl::glLoadIdentity();
	float screenWidth = urb.x - llf.x;
	float screenHeight = urb.y - llf.y;

	gl::glOrtho((llf.x + 1) / screenWidth * (-width),
		width + (1 - urb.x) / screenWidth * width,
		yMin - height * (llf.y + 1) / screenHeight,
		(yMin + height) + height * (1 - urb.y) / screenHeight,
		-1, 1);
}

void glannotations::NVPRRenderer::setupOutline(gl::GLuint& pathSettings, const glannotations::Styling* outline, float scaleFactor /* = 1.f*/) {
	m_drawOutline = false;
	if (outline == nullptr) return;
	m_drawOutline = true;
	const glannotations::Styles::Outline* outlineStyle = reinterpret_cast<const glannotations::Styles::Outline*>(outline);
	gl::glPathParameterfNV(pathSettings, gl::GL_PATH_STROKE_WIDTH_NV, scaleFactor * outlineStyle->getWidth());
}

void glannotations::NVPRRenderer::setupProjection(glm::mat4 projection) const {
	gl::glMatrixMode(gl::GL_PROJECTION);
	gl::glPushMatrix();
	gl::glLoadIdentity();
	gl::glMultMatrixf(glm::value_ptr(projection));
}

void glannotations::NVPRRenderer::setupModelView(glm::mat4 view, const glannotations::QuadState& state, float stencilWidth, float stencilHeight) const {
	gl::glMatrixMode(gl::GL_MODELVIEW);
	gl::glPushMatrix();
	gl::glLoadIdentity();

	float width = state.getLR().x - state.getLL().x;
	float height = state.getUR().y - state.getLR().y;
	glm::mat4 modelMatrix = glm::scale(glm::mat4(1.f), glm::vec3(width / stencilWidth, height / stencilHeight, 1.f));
	//modelMatrix = glm::translate(modelMatrix, glm::vec3(0.f, -yMin, 0.f));

	// rotating according to axes
	/*float xRot = glm::acos(glm::dot(glm::normalize(glm::vec3(0.f, state.getUR().y - state.getLR().y, state.getUR().z - state.getLR().z)), glm::vec3(0.f, 1.f, 0.f)));
	float yRot = glm::acos(glm::dot(glm::normalize(glm::vec3(state.getLR().x - state.getLL().x, 0.f, state.getLR().z - state.getLL().z)), glm::vec3(1.f, 0.f, 0.f)));
	float zRot = glm::acos(glm::dot(glm::normalize(glm::vec3(state.getUR().x - state.getLR().x, state.getUR().y - state.getLR().y, 0.f)), glm::vec3(0.f, 1.f, 0.f)));
	modelMatrix = glm::rotate(modelMatrix, xRot, glm::vec3(1.f, 0.f, 0.f));
	modelMatrix = glm::rotate(modelMatrix, yRot, glm::vec3(0.f, 1.f, 0.f));
	modelMatrix = glm::rotate(modelMatrix, zRot, glm::vec3(0.f, 0.f, 1.f));*/
	modelMatrix = glm::translate(modelMatrix, glm::vec3(state.getLL().x, state.getLL().y, state.getLL().z));
	gl::glMultMatrixf(glm::value_ptr(glm::inverse(view) * modelMatrix));
}

glannotations::NVPRRenderer::NVPRRenderer(gl::GLuint globalMatricesBindingIndex) : AbstractRenderer(globalMatricesBindingIndex) {

}

void glannotations::NVPRRenderer::pushEmptyModelViewMatrix() const {
	gl::glMatrixMode(gl::GL_MODELVIEW);
	gl::glPushMatrix();
	gl::glLoadIdentity();
}

void glannotations::NVPRRenderer::cleanMatrixStacks() const {
	gl::glMatrixMode(gl::GL_MODELVIEW);
	gl::glPopMatrix();
	gl::glMatrixMode(gl::GL_PROJECTION);
	gl::glPopMatrix();
}
