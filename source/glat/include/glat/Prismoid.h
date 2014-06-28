#pragma once

#include <glat/AbstractDrawingPrimitive.h>
#include <vector>

namespace glat {
	class Prismoid : public glat::AbstractDrawingPrimitive {
	public:
		Prismoid(gl::GLuint matricesBindingIndex);
		virtual void draw();
		virtual void setPosition(const std::vector<glm::vec3>& linestrip);

	protected:
		size_t m_numVert;
	};
}