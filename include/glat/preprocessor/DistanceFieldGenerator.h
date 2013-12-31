#ifndef GLAT_PREPROCESSOR_DISTANCE_FIELD_GENERATOR_H
#define GLAT_PREPROCESSOR_DISTANCE_FIELD_GENERATOR_H

#include <glat/PNGImage.h>
#include <glow/ref_ptr.h>

namespace glat {
	namespace preprocessor {
		class DistanceFieldGenerator
		{
		public:
			// set minSideLength to 0 to not downscale given png
			static glow::ref_ptr<glat::PNGImage> distanceTransform(const glat::PNGImage& original, unsigned minSideLength = 0);
			static glat::PNGImage::colorVal_t colorValueFromFloat(double val);
		protected:
			static inline bool selfColoredNeighborsNot(const glat::PNGImage& original, unsigned x, unsigned y, unsigned maxX, unsigned maxY);
			DistanceFieldGenerator() {}
		};
	}
}

#endif // !GLAT_PREPROCESSOR_DISTANCE_FIELD_GENERATOR_H
