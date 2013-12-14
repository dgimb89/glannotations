#ifndef GLAT_QUAD_H
#define GLAT_QUAD_H

#include <glowutils/ScreenAlignedQuad.h>

namespace glat {
	class Quad : public glowutils::ScreenAlignedQuad {
	public:
		Quad(glow::Texture* distanceField);
		void draw();
	private:
		const char * m_vertexShaderSource;
		const char * m_fragmentShaderSource;
	};
}
#endif // !GLAT_QUAD_H
