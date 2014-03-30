#ifndef GLAT_QUAD_STRIP_H
#define GLAT_QUAD_STRIP_H

#include <glat/AbstractDFPrimitive.h>

namespace glat {
	class QuadStrip : public glat::AbstractDFPrimitive {
	public:
		typedef glm::vec2 texVec2_t;
		typedef std::pair<texVec2_t, texVec2_t> textureRange_t;
		QuadStrip(std::shared_ptr<glow::Texture> distanceField);

		void addQuad(texVec2_t texture_ll, texVec2_t texture_advance);
		void clearQuads();

		virtual void draw();
		virtual void setPosition(glm::vec3 ll, glm::vec3 lr, glm::vec3 ur, glm::mat4 modelViewProjection = glm::mat4());

	protected:
		void updateQuadRanges();
		inline void pushTextureCoords(std::vector<texVec2_t>& textureVec, const textureRange_t& textureRange);
		inline texVec2_t getUL(const textureRange_t& textureRange);
		inline texVec2_t getLR(const textureRange_t& textureRange);

		std::vector<textureRange_t> m_textureRanges;
		glm::vec3 m_ll, m_ur, m_lr;
		unsigned m_vertexCount;

		glow::ref_ptr<glow::Buffer> m_advanceH;
		glow::ref_ptr<glow::Buffer> m_advanceW; 
		glow::ref_ptr<glow::Buffer> m_texAdvance;

		glow::ref_ptr<glow::Shader> m_geometryShader;
	};
}
#endif // !GLAT_QUAD_STRIP_H
