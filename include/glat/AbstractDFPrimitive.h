#ifndef GLAT_ABSTRACT_DF_PRIMITIVE_H
#define GLAT_ABSTRACT_DF_PRIMITIVE_H

#include <glat/AbstractDrawingPrimitive.h>

namespace glat {
	class AbstractDFPrimitive : public glat::AbstractDrawingPrimitive {
	public:
		AbstractDFPrimitive(std::shared_ptr<glow::Texture> texture);

		virtual void setOutline(float size, glm::vec3 color);
		virtual void setBumpMap(float intensity);
	protected:
		unsigned m_style;
	};
}

#endif // !GLAT_ABSTRACT_DF_PRIMITIVE_H