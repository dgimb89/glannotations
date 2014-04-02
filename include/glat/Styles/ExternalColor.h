#ifndef GLAT_EXTERNAL_COLOR_H
#define GLAT_EXTERNAL_COLOR_H

#include <glat/Styling.h>
#include <glm/glm.hpp>

namespace glat {
	namespace Styles {
		class ExternalColor : public glat::Styling {
		public:
			ExternalColor(glm::vec4 color);

			void setColor(glm::vec4 color);
			glm::vec4 getColor() const;

		protected:
			glm::vec4 m_color;
		};
	}
}

#endif // !GLAT_EXTERNAL_COLOR_H