#ifndef GLAT_OUTLINE_H
#define GLAT_OUTLINE_H

#include <glat/Styling.h>
#include <glm/glm.hpp>

namespace glat {
	namespace Styles {
		class Outline : public glat::Styling {
		public:
			Outline(float width, glm::vec3 rgbColor);
			float getWidth() const;
			glm::vec3 getColor() const;
			void setWidth(float width);
			void setColor(glm::vec3 rgbColor);

		protected:
			float m_width;
			glm::vec3 m_color;
		};
	}
}

#endif // GLAT_OUTLINE_H
