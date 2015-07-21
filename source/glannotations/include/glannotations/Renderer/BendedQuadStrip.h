#pragma once

#include <glannotations/States/SplineState.h>
#include <glannotations/Renderer/AbstractTexturedPrimitive.h>

namespace glannotations {
	class BendedQuadStrip : public glannotations::AbstractTexturedPrimitive {
	public:
		typedef glm::vec2 texVec2_t;
		typedef std::pair<texVec2_t, texVec2_t> textureRange_t;
		typedef std::pair<glm::vec3, glm::vec3> QuadRange_t;

		BendedQuadStrip(std::shared_ptr<globjects::Texture> texture, gl::GLuint matricesBindingIndex, bool isDistanceField);

		void addQuad(texVec2_t texture_ll, texVec2_t texture_advance, glm::vec3 secantVec, glm::vec3 orthoVec);
		void clearQuads();
		virtual glm::vec2 getExtends() const override;

		virtual void draw();

		float getUniformQuadHeight();
		float getQuadStripWidth();

		virtual bool setPosition(glm::vec3 ll, glm::vec3 lr, glm::vec3 ur) override;
		virtual bool setViewportPosition(glm::vec2 ll, glm::vec2 lr, glm::vec2 ur) override;

		void setTransformationMatrix(glm::mat4 matrix);

	protected:
		void updateQuadPositions();

		std::vector<textureRange_t> m_textureRanges;
		std::vector<QuadRange_t> m_quadRanges;
		//glm::vec3 m_ll, m_ur, m_lr;
		glm::vec3 m_startPoint;
		size_t m_vertexCount;

		globjects::ref_ptr<globjects::Buffer> m_advanceH;
		globjects::ref_ptr<globjects::Buffer> m_advanceW; 
		globjects::ref_ptr<globjects::Buffer> m_texAdvance;

		glm::mat4 m_transformation;
	};
}
