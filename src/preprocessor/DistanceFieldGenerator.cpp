#include <glat/preprocessor/DistanceFieldGenerator.h>
#include <algorithm>

const float kernel[] = {0.f, 	1.f, 		2.f,
						1.f, 	1.4f, 		2.1969f,
						2.f, 	2.1969f, 	2.8f };

#define SWAPIFLOWER(y,x,swapVal) if((x) >= 0 && (x) < original.getWidth() && (y) >= 0 && (y) < original.getHeight()) \
	distances[(y)*original.getWidth() + (x)] = (original.isColored(x, y) ? -1.f : 1.f) * std::min(std::abs(distances[(y)*original.getWidth() + (x)]), std::abs(swapVal))

glow::ref_ptr<glat::PNGImage> glat::preprocessor::DistanceFieldGenerator::distanceTransform(const glat::PNGImage& original, unsigned minSideLength /* = 0 */) {
	float* distances = new float[original.getHeight() * original.getWidth()];
	std::fill_n(distances, original.getHeight() * original.getWidth(), INFINITY);
	// top-left to bottom-right
	for (unsigned y = 0; y < original.getHeight(); ++y) {
		for (unsigned x = 0; x < original.getWidth(); ++x) {
			if (original.isColored(x, y) && distances[y * original.getWidth() + x] > 0.f) {
				distances[y * original.getWidth() + x] = kernel[0];
			}
			// apply kernel
			float currentVal = distances[y * original.getWidth() + x];
			if (currentVal == INFINITY) continue;
			SWAPIFLOWER(y,		x + 1,	currentVal + kernel[1]);
			SWAPIFLOWER(y,		x + 2,	currentVal + kernel[2]);
			SWAPIFLOWER(y + 1,	x,		currentVal + kernel[3]);
			SWAPIFLOWER(y + 1,	x + 1,	currentVal + kernel[4]);
			SWAPIFLOWER(y + 1,	x + 2,	currentVal + kernel[5]);
			SWAPIFLOWER(y + 2,	x,		currentVal + kernel[6]);
			SWAPIFLOWER(y + 2,	x + 1,	currentVal + kernel[7]);
			SWAPIFLOWER(y + 2,	x + 2,	currentVal + kernel[8]);
		}
	}
	// bottom-right to top-left
	for (unsigned y = original.getHeight(); y > 0; --y) {
		for (unsigned x = original.getWidth(); x > 0; --x) {
			if (original.isColored(x - 1, y - 1)) {
				distances[(y - 1) * original.getWidth() + (x - 1)] = kernel[0];
			}
			// apply kernel
			float currentVal = distances[(y - 1) * original.getWidth() + (x - 1)];
			SWAPIFLOWER(y - 1,	x - 2,	currentVal + kernel[1]);
			SWAPIFLOWER(y - 1,	x - 3,	currentVal + kernel[2]);
			SWAPIFLOWER(y - 2,	x - 1,	currentVal + kernel[3]);
			SWAPIFLOWER(y - 2,	x - 2,	currentVal + kernel[4]);
			SWAPIFLOWER(y - 2,	x - 3,	currentVal + kernel[5]);
			SWAPIFLOWER(y - 3,	x - 1,	currentVal + kernel[6]);
			SWAPIFLOWER(y - 3,	x - 2,	currentVal + kernel[7]);
			SWAPIFLOWER(y - 3,	x - 3,	currentVal + kernel[8]);
		}
	}
	// normalize distances
	float maxDistance = INFINITY;

	// image has no colored pixel if any distance is still infinity
	if (distances[0] != INFINITY) {
		for (unsigned i = original.getHeight() * original.getWidth(); i > 0; --i) {
			maxDistance = std::max(maxDistance, distances[i - 1]);
		}

	}

	// transform float distances to color values

	glow::ref_ptr<glat::PNGImage> distanceField = new glat::PNGImage(original.getWidth(), original.getHeight(), 1);
	return distanceField;
}

glat::PNGImage::colorVal_t glat::preprocessor::DistanceFieldGenerator::colorValueFromFloat(float val) {
	float clampedVal = std::max(0.f, std::min(1.f, val));
	return static_cast<glat::PNGImage::colorVal_t>(floorf(clampedVal == 1.f ? 255.f : clampedVal * 256.f));
}