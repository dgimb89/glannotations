#ifndef GLAT_BUMPMAP_H
#define GLAT_BUMPMAP_H

#include <glat/Styling.h>

namespace glat {
	namespace Styles {
		class BumpMap : public glat::Styling {
		public:
			BumpMap(float intensity);
			float getIntensity() const;
			void setIntensity(float intensity);

		protected:
			float m_intensity;
		};
	}
}

#endif // GLAT_BUMPMAP_H
