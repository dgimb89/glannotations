#pragma once

#include <glat/AbstractTexturedPrimitive.h>

namespace glat {
	class QuadStrip : public glat::AbstractTexturedPrimitive {
	public:
		typedef glm::vec2 texVec2_t;
		typedef std::pair<texVec2_t, texVec2_t> textureRange_t;
		QuadStrip(std::shared_ptr<glow::Texture> texture, gl::GLuint matricesBindingIndex, bool isDistanceField);

		void addQuad(texVec2_t texture_ll, texVec2_t texture_advance);
		void clearQuads();

		float getUniformQuadHeight();
		size_t getQuadstripRowCount();
		float getQuadStripHeight();
		float getQuadStripWidth();

		virtual void draw();
		virtual void setPosition(glm::vec3 ll, glm::vec3 lr, glm::vec3 ur);

	protected:
		void updateQuadRanges();
		void pushTextureCoords(std::vector<texVec2_t>& textureVec, const textureRange_t& textureRange);
		texVec2_t getUL(const textureRange_t& textureRange);
		texVec2_t getLR(const textureRange_t& textureRange);

		std::vector<textureRange_t> m_textureRanges;
		glm::vec3 m_ll, m_ur, m_lr;
		unsigned m_vertexCount;

		glow::ref_ptr<glow::Buffer> m_advanceH;
		glow::ref_ptr<glow::Buffer> m_advanceW; 
		glow::ref_ptr<glow::Buffer> m_texAdvance;
	};
}
