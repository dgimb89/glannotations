#pragma once

#include <glow/Shader.h>
#include <glow/Program.h>
#include <glow/VertexArrayObject.h>
#include <glow/Buffer.h>

#include <glat/AbstractDrawingPrimitive.h>

namespace glat {
	class GLAT_API DrawingPrimitiveBase : public AbstractDrawingPrimitive {
	public:
		DrawingPrimitiveBase();
		virtual ~DrawingPrimitiveBase();

		void setupShader(const char* vertShader, const char* fragShader);
		void setupShader(const char* vertShader, const char* geomShader, const char* fragShader);

		virtual void setColor(glm::vec4 color) override;
		virtual void setBindingIndex(unsigned int bindingIndex) override;

	protected:
		glow::Shader* replacePlaceholderAndGetShader(gl::GLenum shaderType, const char* shader);

		glow::ref_ptr<glow::Program> m_program;
		glow::ref_ptr<glow::VertexArrayObject> m_vao;
		glow::ref_ptr<glow::Buffer> m_positions;
	};
}