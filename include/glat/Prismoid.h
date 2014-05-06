#pragma once

#include <glat/AbstractDrawingPrimitive.h>
#include <vector>

namespace glat {
	class Prismoid : public glat::AbstractDrawingPrimitive {
	public:
		Prismoid();
		virtual void draw();
		virtual void setPosition(const std::vector<glm::vec3>& linestrip, glm::mat4 modelViewProjection = glm::mat4());

	protected:
		size_t m_numVert;
		glow::ref_ptr<glow::Shader> m_geomShader;
	};
}