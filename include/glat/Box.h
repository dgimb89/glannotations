#ifndef GLAT_BOX_H
#define GLAT_BOX_H

#include <glat/AbstractDrawingPrimitive.h>

namespace glat {
	class Box : public glat::AbstractDrawingPrimitive {
	public:
		Box();
		virtual void draw();
		void setPosition(glm::vec3 llf, glm::vec3 widthSpan, glm::vec3 heightSpan, glm::vec3 depthSpan);

	protected:
		glow::ref_ptr<glow::Shader> m_geometryShader;

		glow::ref_ptr<glow::Buffer> m_widthSpan;
		glow::ref_ptr<glow::Buffer> m_heightSpan;
		glow::ref_ptr<glow::Buffer> m_depthSpan;
	};
}

#endif // !GLAT_BOX_H