#pragma once

#include <glat/AbstractTexturedPrimitive.h>

namespace glat {
	class QuadStrip : public glat::AbstractTexturedPrimitive {
	public:
		typedef glm::vec2 texVec2_t;
		typedef std::pair<texVec2_t, texVec2_t> textureRange_t;
		QuadStrip(std::shared_ptr<glo::Texture> texture, gl::GLuint matricesBindingIndex, bool isDistanceField);

		void addQuad(texVec2_t texture_ll, texVec2_t texture_advance);
		void clearQuads();

		float getUniformQuadHeight();
		size_t getQuadstripRowCount();
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

		glo::ref_ptr<glo::Buffer> m_advanceH;
		glo::ref_ptr<glo::Buffer> m_advanceW; 
		glo::ref_ptr<glo::Buffer> m_texAdvance;
	};
}
