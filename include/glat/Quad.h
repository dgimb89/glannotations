#ifndef GLAT_QUAD_H
#define GLAT_QUAD_H

#include <glowutils/ScreenAlignedQuad.h>
#include <glm/glm.hpp>

namespace glat {
	class Quad : public glowutils::ScreenAlignedQuad {
	public:
		Quad(glow::Texture* distanceField);
		void draw();
		void setScale(glm::vec2 scale);
		void setOffset(glm::vec2 offset);
		void setOutline(float size, glm::vec3 color);
		void setBumpMap(float intensity);
	private:
		int m_style;
	};
}
#endif // !GLAT_QUAD_H
