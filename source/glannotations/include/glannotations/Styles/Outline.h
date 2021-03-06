#ifndef GLAT_OUTLINE_H
#define GLAT_OUTLINE_H

#include <glm/glm.hpp>

#include <glannotations/Styles/Styling.h>
#include <glannotations/glannotations_api.h>

namespace glannotations {
	namespace Styles {
		class GLANNOTATIONS_API Outline : public glannotations::Styling {
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
