#pragma once

#include <glat/PNGImage.h>
#include <glow/ref_ptr.h>

namespace glat {
	class DistanceFieldGenerator
	{
	public:
		// set minSideLength to 0 to not downscale given png
		static glow::ref_ptr<glat::PNGImage> distanceTransform(const glat::PNGImage& original);
		static glow::ref_ptr<glat::PNGImage> scaleImageToWidth(const glat::PNGImage& original, unsigned scaledWidth);
		static glow::ref_ptr<glat::PNGImage> scaleImageToHeight(const glat::PNGImage& original, unsigned scaledHeight);
		static glow::ref_ptr<glat::PNGImage> scaleImage(const glat::PNGImage& original, float scaleFactor);

		static glat::PNGImage::colorVal_t colorValueFromFloat(double val);
		static glow::ref_ptr<glat::PNGImage> bilinearResize(const glat::PNGImage& inImage, unsigned scaledWidth, unsigned scaledHeight);
		static glow::ref_ptr<glat::PNGImage> bicubicResize(const glat::PNGImage& inImage, unsigned scaledWidth, unsigned scaledHeight);
	protected:
		static inline bool selfColoredNeighborsNot(const glat::PNGImage& original, unsigned x, unsigned y);
		DistanceFieldGenerator() {}
	};
}
