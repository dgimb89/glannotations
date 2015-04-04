#pragma once

#include <globjects/Shader.h>
#include <globjects/Program.h>
#include <globjects/VertexArray.h>
#include <globjects/Buffer.h>

#include <glannotations/AbstractDrawingPrimitive.h>

namespace glannotations {
	class GLANNOTATIONS_API DrawingPrimitiveBase : public AbstractDrawingPrimitive {
	public:
		DrawingPrimitiveBase();
		virtual ~DrawingPrimitiveBase();
		void setupShader(const char* vertShader, const char* geomShader, const char* fragShader);

		virtual void setColor(glm::vec4 color) override;
		virtual void setBindingIndex(unsigned int bindingIndex) override;

	protected:
		void initialize();

		globjects::Shader* finalizeGeometryShader(const char* shader);

		globjects::ref_ptr<globjects::Program> m_program;
		globjects::ref_ptr<globjects::VertexArray> m_vao;
		globjects::ref_ptr<globjects::Buffer> m_positions;
	};
}