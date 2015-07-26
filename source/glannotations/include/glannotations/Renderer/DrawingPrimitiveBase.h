#pragma once

#include <globjects/Shader.h>
#include <globjects/Program.h>
#include <globjects/VertexArray.h>
#include <globjects/Buffer.h>

#include <glannotations/Renderer/AbstractDrawingPrimitive.h>

namespace glannotations {
	class GLANNOTATIONS_API DrawingPrimitiveBase : public AbstractDrawingPrimitive {
	public:
		DrawingPrimitiveBase();
		virtual ~DrawingPrimitiveBase();
        void setupShader(const std::string & vertShader, const std::string & geomShader, const std::string & fragShader);

		virtual void setColor(glm::vec4 color) override;
		virtual void setBindingIndex(unsigned int bindingIndex) override;

		void directDrawCall();

	protected:
		void initialize();

        globjects::Shader* finalizeGeometryShader(const std::string & shader);

		globjects::ref_ptr<globjects::Program> m_program;
		globjects::ref_ptr<globjects::VertexArray> m_vao;
		globjects::ref_ptr<globjects::Buffer> m_positions;

		size_t m_vertexCount = 0;
	};
}
