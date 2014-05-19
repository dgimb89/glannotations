#include <glat/NVPRRenderer.h>
#include <glat/AbstractAnnotation.h>
#include <glat/Styles/Outline.h>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

void glat::NVPRRenderer::draw(glat::AbstractAnnotation* annotation) {
	// enable stencil test as needed by nvpr
	glEnable(GL_STENCIL_TEST);
	// TODO: do we have to clear stencil buffer with every draw?
	glStencilFunc(GL_NOTEQUAL, 0, 0x1F);
	glStencilOp(GL_KEEP, GL_KEEP, GL_ZERO);
	// double dispatch to draw specific state
	annotation->getState()->draw(*this);
	// disable stencil test when finished
	glDisable(GL_STENCIL_TEST);
}

void glat::NVPRRenderer::clearStencilBuffer() {
	glClearStencil(0);
	glStencilMask(~0);
	glClear(GL_STENCIL_BUFFER_BIT);
}

glat::NVPRRenderer::NVPRRenderer() {
	initializeNVPR();
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

void glat::NVPRRenderer::setupOutline(GLuint& pathSettings, const glat::Styling* outline, float scaleFactor /* = 1.f*/) {
	m_drawOutline = false;
	if (outline == nullptr) return;
	m_drawOutline = true;
	const glat::Styles::Outline* outlineStyle = reinterpret_cast<const glat::Styles::Outline*>(outline);
	glPathParameterfNV(pathSettings, GL_PATH_STROKE_WIDTH_NV, scaleFactor * outlineStyle->getWidth());
}

void glat::NVPRRenderer::setupInternalProjection(glm::mat4 mvp, glm::vec3 ll) const {
	glMultMatrixf(glm::value_ptr(glm::translate(mvp, glm::vec3(ll.x, ll.y * 0.5, ll.z))));
}

void glat::NVPRRenderer::setupInternalModelview(glm::vec3 ll, glm::vec3 lr, glm::vec3 ur, float stencilWidth, float stencilHeight) const {
	float width = lr.x - ll.x;
	float height = ur.y - lr.y;
	glm::mat4 modelMatrix = glm::scale(glm::mat4(1.f), glm::vec3(width / stencilWidth, height / stencilHeight, 1.f));
	//modelMatrix = glm::translate(modelMatrix, glm::vec3(0.f, -yMin, 0.f));

	// rotating according to axes
	/*float xRot = glm::acos(glm::dot(glm::normalize(glm::vec3(0.f, state.getUR().y - state.getLR().y, state.getUR().z - state.getLR().z)), glm::vec3(0.f, 1.f, 0.f)));
	float yRot = glm::acos(glm::dot(glm::normalize(glm::vec3(state.getLR().x - state.getLL().x, 0.f, state.getLR().z - state.getLL().z)), glm::vec3(1.f, 0.f, 0.f)));
	float zRot = glm::acos(glm::dot(glm::normalize(glm::vec3(state.getUR().x - state.getLR().x, state.getUR().y - state.getLR().y, 0.f)), glm::vec3(0.f, 1.f, 0.f)));
	modelMatrix = glm::rotate(modelMatrix, xRot, glm::vec3(1.f, 0.f, 0.f));
	modelMatrix = glm::rotate(modelMatrix, yRot, glm::vec3(0.f, 1.f, 0.f));
	modelMatrix = glm::rotate(modelMatrix, zRot, glm::vec3(0.f, 0.f, 1.f));*/
	glMultMatrixf(glm::value_ptr(modelMatrix));
}
