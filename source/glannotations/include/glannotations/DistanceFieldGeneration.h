#pragma once

#include <globjects/base/ref_ptr.h>

#include <glannotations/PNGImage.h>

namespace glat {
	namespace DistanceFieldGeneration {
		globjects::ref_ptr<glat::PNGImage> distanceTransform(const glat::PNGImage& original);
		globjects::ref_ptr<glat::PNGImage> scaleImageToWidth(const glat::PNGImage& original, unsigned scaledWidth);
		globjects::ref_ptr<glat::PNGImage> scaleImageToHeight(const glat::PNGImage& original, unsigned scaledHeight);
		globjects::ref_ptr<glat::PNGImage> scaleImage(const glat::PNGImage& original, float scaleFactor);

		glat::PNGImage::colorVal_t colorValueFromFloat(double val);
		globjects::ref_ptr<glat::PNGImage> bilinearResize(const glat::PNGImage& inImage, unsigned scaledWidth, unsigned scaledHeight);
		globjects::ref_ptr<glat::PNGImage> bicubicResize(const glat::PNGImage& inImage, unsigned scaledWidth, unsigned scaledHeight);

		namespace detail {
			bool selfColoredNeighborsNot(const glat::PNGImage& original, unsigned x, unsigned y);
		}
	}
}
