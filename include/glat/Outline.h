#ifndef GLAT_OUTLINE_H
#define GLAT_OUTLINE_H

#include <glat/Styling.h>
#include <glm/glm.hpp>

namespace glat {
	namespace Style {
		class Outline : public glat::Styling {
		public:
			Outline(float width, glm::vec4 rgbaColor);
			void setWidth(float width);
			void setColor(glm::vec4 rgbaColor);

		protected:
			float m_width;
			glm::vec4 m_color;
		};
	}
}

#endif // GLAT_OUTLINE_H
