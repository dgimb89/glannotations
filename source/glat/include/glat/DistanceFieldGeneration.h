#pragma once

#include <glowbase/ref_ptr.h>

#include <glat/PNGImage.h>

namespace glat {
	namespace DistanceFieldGeneration {
		glow::ref_ptr<glat::PNGImage> distanceTransform(const glat::PNGImage& original);
		glow::ref_ptr<glat::PNGImage> scaleImageToWidth(const glat::PNGImage& original, unsigned scaledWidth);
		glow::ref_ptr<glat::PNGImage> scaleImageToHeight(const glat::PNGImage& original, unsigned scaledHeight);
		glow::ref_ptr<glat::PNGImage> scaleImage(const glat::PNGImage& original, float scaleFactor);

		glat::PNGImage::colorVal_t colorValueFromFloat(double val);
		glow::ref_ptr<glat::PNGImage> bilinearResize(const glat::PNGImage& inImage, unsigned scaledWidth, unsigned scaledHeight);
		glow::ref_ptr<glat::PNGImage> bicubicResize(const glat::PNGImage& inImage, unsigned scaledWidth, unsigned scaledHeight);

		namespace detail {
			bool selfColoredNeighborsNot(const glat::PNGImage& original, unsigned x, unsigned y);
		}
	}
}
