#pragma once

#include <glannotations/AbstractTexturedPrimitive.h>

namespace glannotations {
	class QuadStrip : public glannotations::AbstractTexturedPrimitive {
	public:
		typedef glm::vec2 texVec2_t;
		typedef std::pair<texVec2_t, texVec2_t> textureRange_t;
		QuadStrip(std::shared_ptr<globjects::Texture> texture, gl::GLuint matricesBindingIndex, bool isDistanceField);

		void addQuad(texVec2_t texture_ll, texVec2_t texture_advance);
		void clearQuads();
		virtual glm::vec2 getExtends() const override;

		float getUniformQuadHeight();
		size_t getQuadstripRowCount() const;
		float getQuadStripHeight();
		float getQuadStripWidth();

		virtual void draw();
		virtual bool setPosition(glm::vec3 ll, glm::vec3 lr, glm::vec3 ur) override;
		virtual bool setViewportPosition(glm::vec2 ll, glm::vec2 lr, glm::vec2 ur) override;

	protected:
		void updateQuadRanges();
		void pushTextureCoords(std::vector<texVec2_t>& textureVec, const textureRange_t& textureRange);
		bool positionValid(const glm::vec3& ll, const glm::vec3& lr, const glm::vec3& ur) const;
		texVec2_t getUL(const textureRange_t& textureRange);
		texVec2_t getLR(const textureRange_t& textureRange);

		std::vector<textureRange_t> m_textureRanges;
		glm::vec3 m_ll, m_ur, m_lr;
		unsigned m_vertexCount;

		globjects::ref_ptr<globjects::Buffer> m_advanceH;
		globjects::ref_ptr<globjects::Buffer> m_advanceW; 
		globjects::ref_ptr<globjects::Buffer> m_texAdvance;
	};
}
