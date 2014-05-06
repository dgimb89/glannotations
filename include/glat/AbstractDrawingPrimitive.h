#pragma once

#include <glat/DirtyFlagObject.h>

#include <glm/glm.hpp>

#include <glow/Shader.h>
#include <glow/Program.h>
#include <glow/VertexArrayObject.h>
#include <glow/Buffer.h>

namespace glat {
	class AbstractDrawingPrimitive : public glow::Referenced {
	public:
		AbstractDrawingPrimitive();
		virtual ~AbstractDrawingPrimitive();

		virtual void draw() = 0;
		virtual void setColor(glm::vec4 color);

		virtual void setModelViewProjection(glm::mat4 modelViewProjection = glm::mat4());
		void setupShader(const char* fragShader, const char* vertShader);

	protected:
		void initialize();

		glow::ref_ptr<glow::Shader> m_vertexShader;
		glow::ref_ptr<glow::Shader> m_fragmentShader;

		glow::ref_ptr<glow::Program> m_program;

		glow::ref_ptr<glow::VertexArrayObject> m_vao;
		glow::ref_ptr<glow::Buffer> m_positions;
	};
}
