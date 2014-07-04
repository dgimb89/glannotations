#pragma once

#include <glow/Shader.h>
#include <glow/Program.h>
#include <glow/VertexArrayObject.h>
#include <glow/Buffer.h>
#include <glm/glm.hpp>

#include <glat/DirtyFlagObject.h>
#include <glat/glat_api.h>

namespace glat {
	class GLAT_API AbstractDrawingPrimitive : public glow::Referenced {
	public:
		AbstractDrawingPrimitive();
		virtual ~AbstractDrawingPrimitive();

		virtual void draw() = 0;
		void setColor(glm::vec4 color);
		// important: bind matrices AFTER you've setup shader
		void setMatricesBlockBinding(gl::GLuint bindingIndex);
		void setupShader(const char* vertShader, const char* fragShader);
		void setupShader(const char* vertShader, const char* geomShader, const char* fragShader);

	protected:
		glow::Shader* replacePlaceholderAndGetShader(gl::GLenum shaderType, const char* shader);
		void initialize();

		glow::ref_ptr<glow::Program> m_program;

		glow::ref_ptr<glow::VertexArrayObject> m_vao;
		glow::ref_ptr<glow::Buffer> m_positions;
	};
}
