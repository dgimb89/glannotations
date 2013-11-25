#ifndef GLAT_QUAD_H
#define GLAT_QUAD_H

#include <glowutils/ScreenAlignedQuad.h>

namespace glat {
	class Quad : public glow::ScreenAlignedQuad {
	public:
		Quad(glow::Shader* fragmentShader);
		void draw();
	};
}
#endif // !GLAT_QUAD_H
