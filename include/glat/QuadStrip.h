#ifndef GLAT_QUAD_STRIP_H
#define GLAT_QUAD_STRIP_H

#include <glat/AbstractDrawingPrimitive.h>

namespace glat {
	class QuadStrip : public glat::AbstractDrawingPrimitive{
	public:
		typedef std::pair<glm::vec2, glm::vec2> textureRange_t;
		QuadStrip(std::shared_ptr<glow::Texture> distanceField);

		void addQuad(glm::vec2 texture_ll, glm::vec2 texture_ur);
		void clearQuads();

		virtual void draw();
		virtual void setPosition(glm::vec3 llf, glm::vec3 urb, glm::mat4 modelViewProjection = glm::mat4());

	protected:
		void updateQuadRanges();
		inline void pushTextureCoords(std::vector<glm::vec2>& textureVec, const textureRange_t& textureRange);
		inline glm::vec2 getUL(const textureRange_t& textureRange);
		inline glm::vec2 getLR(const textureRange_t& textureRange);

		std::vector<textureRange_t> m_textureRanges;
		glm::vec3 m_llf;
		glm::vec3 m_urb;
		unsigned m_vertexCount;

		glow::ref_ptr<glow::Buffer> m_secondTexCoords;
		glow::ref_ptr<glow::Buffer> m_texSwitch;
	};
}
#endif // !GLAT_QUAD_STRIP_H
