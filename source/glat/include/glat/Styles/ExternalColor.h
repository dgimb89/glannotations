#ifndef GLAT_EXTERNAL_COLOR_H
#define GLAT_EXTERNAL_COLOR_H

#include <glm/glm.hpp>

#include <glat/Styling.h>
#include <glat/glat_api.h>

namespace glat {
	namespace Styles {
		class GLAT_API ExternalColor : public glat::Styling {
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