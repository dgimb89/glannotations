#pragma once

#include <globjects-base/ref_ptr.h>

#include <glat/PNGImage.h>

namespace glat {
	namespace DistanceFieldGeneration {
		glo::ref_ptr<glat::PNGImage> distanceTransform(const glat::PNGImage& original);
		glo::ref_ptr<glat::PNGImage> scaleImageToWidth(const glat::PNGImage& original, unsigned scaledWidth);
		glo::ref_ptr<glat::PNGImage> scaleImageToHeight(const glat::PNGImage& original, unsigned scaledHeight);
		glo::ref_ptr<glat::PNGImage> scaleImage(const glat::PNGImage& original, float scaleFactor);

		glat::PNGImage::colorVal_t colorValueFromFloat(double val);
		glo::ref_ptr<glat::PNGImage> bilinearResize(const glat::PNGImage& inImage, unsigned scaledWidth, unsigned scaledHeight);
		glo::ref_ptr<glat::PNGImage> bicubicResize(const glat::PNGImage& inImage, unsigned scaledWidth, unsigned scaledHeight);

		namespace detail {
			bool selfColoredNeighborsNot(const glat::PNGImage& original, unsigned x, unsigned y);
		}
	}
}
