#pragma once

#include <globjects/base/ref_ptr.h>

#include <glannotations/Common/PNGImage.h>

namespace glannotations {
	namespace DistanceFieldGeneration {
		globjects::ref_ptr<glannotations::PNGImage> distanceTransform(const glannotations::PNGImage& original);
		globjects::ref_ptr<glannotations::PNGImage> scaleImageToWidth(const glannotations::PNGImage& original, unsigned scaledWidth);
		globjects::ref_ptr<glannotations::PNGImage> scaleImageToHeight(const glannotations::PNGImage& original, unsigned scaledHeight);
		globjects::ref_ptr<glannotations::PNGImage> scaleImage(const glannotations::PNGImage& original, float scaleFactor);

		glannotations::PNGImage::colorVal_t colorValueFromFloat(double val);
		globjects::ref_ptr<glannotations::PNGImage> bilinearResize(const glannotations::PNGImage& inImage, unsigned scaledWidth, unsigned scaledHeight);
		globjects::ref_ptr<glannotations::PNGImage> bicubicResize(const glannotations::PNGImage& inImage, size_t scaledWidth, size_t scaledHeight);

		namespace detail {
			bool selfColoredNeighborsNot(const glannotations::PNGImage& original, unsigned x, unsigned y);
		}
	}
}
