#ifndef GLAT_QUAD_H
#define GLAT_QUAD_H

#include <glat/AbstractDFPrimitive.h>

namespace glat {
	class Quad : public glat::AbstractDFPrimitive {
	public:
		Quad(std::shared_ptr<glow::Texture> texture, bool isDistanceField);
		
		virtual void draw();
		virtual void setPosition(glm::vec3 ll, glm::vec3 lr, glm::vec3 ur, glm::mat4 modelViewProjection = glm::mat4());
	};
}
#endif // !GLAT_QUAD_H
