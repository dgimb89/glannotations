#include <glat/Quad.h>

glat::Quad::Quad(glow::Shader* fragmentShader) : glow::ScreenAlignedQuad(fragmentShader) {
}

void glat::Quad::draw() {
	glow::ScreenAlignedQuad::draw();
}