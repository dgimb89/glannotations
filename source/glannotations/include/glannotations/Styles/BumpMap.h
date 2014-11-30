#ifndef GLAT_BUMPMAP_H
#define GLAT_BUMPMAP_H

#include <glannotations/Styling.h>
#include <glannotations/glannotations_api.h>

namespace glannotations {
	namespace Styles {
		class GLANNOTATIONS_API BumpMap : public glannotations::Styling {
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
