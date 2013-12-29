#include <glat/preprocessor/DistanceFieldGenerator.h>
#include <algorithm>

const float kernel[] = {0.f,	1.f,		2.f,
						1.f,	1.4f,		2.1969f,
						2.f,	2.1969f,	2.8f};

glow::ref_ptr<glat::PNGImage> glat::preprocessor::DistanceFieldGenerator::distanceTransform(const glat::PNGImage& original, unsigned minSideLength = 0) {
	float* topdownPass = new float[original.getHeight() * original.getWidth()];
	float* bottomupPass = new float[original.getHeight() * original.getWidth()];
	// top-left to bottom-right
	for (unsigned y = 0; y < original.getHeight; ++y) {
		for (unsigned x = 0; x < original.getWidth(); ++x) {
			if (original.isColored(x, y)) {
				// apply kernel
				// to do : prevent memory violation
				topdownPass[y * original.getWidth() + x] = kernel[0];
				topdownPass[y * original.getWidth() + x + 1] = kernel[1];
				topdownPass[y * original.getWidth() + x + 2] = kernel[2];
				topdownPass[(y + 1) * original.getWidth() + x] = kernel[3];
				topdownPass[(y + 1) * original.getWidth() + x + 1] = kernel[4];
				topdownPass[(y + 1) * original.getWidth() + x + 2] = kernel[5];
				topdownPass[(y + 2) * original.getWidth() + x] = kernel[6];
				topdownPass[(y + 2) * original.getWidth() + x + 1] = kernel[7];
				topdownPass[(y + 2) * original.getWidth() + x + 2] = kernel[8];
			}
		}
	}
	// bottom-right to top-left
	// normalize distances
	// transform float distances

	glow::ref_ptr<glat::PNGImage> distanceField = new glat::PNGImage(original.getWidth(), original.getHeight(), 1);
	return distanceField;
}

glat::PNGImage::image_t glat::preprocessor::DistanceFieldGenerator::colorValueFromFloat(float val) {
	float clampedVal = std::max(0.f, std::min(1.f, val));
	return static_cast<glat::PNGImage::image_t>(floorf(clampedVal == 1.f ? 255 : clampedVal * 256.0));
}