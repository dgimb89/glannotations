#ifndef GLAT_EXTERNAL_COLOR_H
#define GLAT_EXTERNAL_COLOR_H

#include <glm/glm.hpp>

#include <glannotations/Styling.h>
#include <glannotations/glannotations_api.h>

namespace glannotations {
	namespace Styles {
		class GLANNOTATIONS_API ExternalColor : public glannotations::Styling {
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