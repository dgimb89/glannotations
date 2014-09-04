#pragma once

#include <globjects/Shader.h>
#include <globjects/Program.h>
#include <globjects/VertexArrayObject.h>
#include <globjects/Buffer.h>

#include <glat/AbstractDrawingPrimitive.h>

namespace glat {
	class GLAT_API DrawingPrimitiveBase : public AbstractDrawingPrimitive {
	public:
		DrawingPrimitiveBase();
		virtual ~DrawingPrimitiveBase();

		void setupShader(const char* vertShader, const char* geomShader, const char* fragShader);

		virtual void setColor(glm::vec4 color) override;
		virtual void setBindingIndex(unsigned int bindingIndex) override;

	protected:
		glo::Shader* finalizeGeometryShader(const char* shader);

		glo::ref_ptr<glo::Program> m_program;
		glo::ref_ptr<glo::VertexArrayObject> m_vao;
		glo::ref_ptr<glo::Buffer> m_positions;
	};
}