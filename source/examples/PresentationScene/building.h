#pragma once

#include <glat/AbstractDrawingPrimitive.h>

namespace glat {
	class Building : public glat::AbstractDrawingPrimitive {
	public:
		Building();
		virtual void draw();
		void setPosition(glm::vec3 llf, glm::vec3 urb);

	protected:
		glow::ref_ptr<glow::Shader> m_geometryShader;
	};
}