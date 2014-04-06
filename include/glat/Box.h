#pragma once

#include <glat/AbstractDrawingPrimitive.h>

namespace glat {
	class Box : public glat::AbstractDrawingPrimitive {
	public:
		Box();
		virtual void draw();
		void setPosition(glm::vec3 llf, glm::vec3 urb);

	protected:
		glow::ref_ptr<glow::Shader> m_geometryShader;
	};
}