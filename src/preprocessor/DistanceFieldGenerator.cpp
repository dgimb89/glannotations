#include <glat/preprocessor/DistanceFieldGenerator.h>
#include <algorithm>

const float kernel[] = {0.f, 	1.f, 		2.f,
						1.f, 	1.4f, 		2.1969f,
						2.f, 	2.1969f, 	2.8f };

#define SWAPIFLOWER(y,x,swapVal) if((x) >= 0 && (x) < original.getWidth() && (y) >= 0 && (y) < original.getHeight()) \
	distances[(y)*original.getWidth() + (x)] = (original.isColored(x, y) ? -1.0 : 1.0) * std::min(std::abs(distances[(y)*original.getWidth() + (x)]), std::abs(swapVal))

bool glat::preprocessor::DistanceFieldGenerator::selfColoredNeighborsNot(const glat::PNGImage& original, unsigned x, unsigned y, unsigned maxX, unsigned maxY) {
	return original.isColored(x, y) && 
		((x > 0 ? !original.isColored(x - 1, y) : true) || (y > 0 ? !original.isColored(x, y - 1) : true) || 
		(x < maxX ? !original.isColored(x + 1, y) : true) || (y < maxY ? !original.isColored(x, y + 1) : true));
}

// bilinear interpolation
glow::ref_ptr<glat::PNGImage> bilinearResize(const glat::PNGImage& inImage, unsigned scaledWidth, unsigned scaledHeight) {
	glow::ref_ptr<glat::PNGImage> scaledResult = new glat::PNGImage(scaledWidth, scaledHeight, inImage.getNumComponents());

	const double tx = double(inImage.getWidth()) / scaledWidth;
	const double ty = double(inImage.getHeight()) / scaledHeight;
	const unsigned channels = inImage.getNumComponents();

	for (unsigned y = 0; y < scaledHeight; ++y){
		for (unsigned x = 0; x < scaledWidth; ++x) {
			double xPart = (x * tx) - std::floor(x * tx);
			double yPart = (y * ty) - std::floor(y * ty);
			double R_1 = ((1.0 - xPart) * inImage.getImageValue(std::floor(x * tx), std::floor(y * ty), 1)) + (xPart * inImage.getImageValue(std::ceil(x * tx), std::floor(y * ty), 1));
			double R_2 = ((1.0 - xPart) * inImage.getImageValue(std::floor(x * tx), std::ceil(y * ty), 1)) + (xPart * inImage.getImageValue(std::ceil(x * tx), std::ceil(y * ty), 1));

			scaledResult->setImageValue(x, y, 0, static_cast<glat::PNGImage::colorVal_t>((1.0 - yPart) * R_1 + (yPart * R_2)));
		}
	}

	return scaledResult;
}
// ---

glow::ref_ptr<glat::PNGImage> glat::preprocessor::DistanceFieldGenerator::distanceTransform(const glat::PNGImage& original, float scalingFactor) {
	return bilinearResize(*distanceTransform(original), original.getWidth() * scalingFactor, original.getHeight() * scalingFactor);
}

glow::ref_ptr<glat::PNGImage> glat::preprocessor::DistanceFieldGenerator::distanceTransform(const glat::PNGImage& original, unsigned minSideLength) {
	return (original.getHeight() > original.getWidth()) ? 
		bilinearResize(*distanceTransform(original), minSideLength, original.getHeight() / original.getWidth() * minSideLength) :
		bilinearResize(*distanceTransform(original), original.getWidth() * minSideLength / original.getHeight(), minSideLength);
}

glow::ref_ptr<glat::PNGImage> glat::preprocessor::DistanceFieldGenerator::distanceTransform(const glat::PNGImage& original, unsigned scaledWidth, unsigned scaledHeight) {
	return bilinearResize(*distanceTransform(original), scaledWidth, scaledHeight);
}

glow::ref_ptr<glat::PNGImage> glat::preprocessor::DistanceFieldGenerator::distanceTransform(const glat::PNGImage& original) {
	double* distances = new double[original.getHeight() * original.getWidth()];
	std::fill_n(distances, original.getHeight() * original.getWidth(), INFINITY);
	// top-left to bottom-right
	for (unsigned y = 0; y < original.getHeight(); ++y) {
		for (unsigned x = 0; x < original.getWidth(); ++x) {
			if (selfColoredNeighborsNot(original, x, y, original.getWidth() - 1, original.getHeight() - 1)) {
				distances[y * original.getWidth() + x] = kernel[0];
			}
			// apply kernel
			double currentVal = std::abs(distances[y * original.getWidth() + x]);
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
			if (selfColoredNeighborsNot(original, x - 1, y - 1, original.getWidth() - 1, original.getHeight() - 1)) {
				distances[(y - 1) * original.getWidth() + (x - 1)] = kernel[0];
			}
			// apply kernel
			double currentVal = std::abs(distances[(y - 1) * original.getWidth() + (x - 1)]);
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
	double maxDistance = 0.f;

	// image has no colored pixel if any distance is still infinity
	if (distances[0] != INFINITY) {
		for (unsigned i = original.getHeight() * original.getWidth(); i > 0; --i) {
			maxDistance = std::max(maxDistance, std::abs(distances[i - 1]));
		}
		for (unsigned i = original.getHeight() * original.getWidth(); i > 0; --i) {
			distances[i - 1] /= maxDistance;
		}
	}

	// transform float distances to color values

	glow::ref_ptr<glat::PNGImage> distanceField = new glat::PNGImage(original.getWidth(), original.getHeight(), 1);
	for (unsigned y = 0; y < original.getHeight(); ++y) {
		for (unsigned x = 0; x < original.getWidth(); ++x) {
			distanceField->setImageValue(x, y, 0, colorValueFromFloat(distances[y * original.getWidth() + x]));
		}
	}
	return distanceField;
}

glat::PNGImage::colorVal_t glat::preprocessor::DistanceFieldGenerator::colorValueFromFloat(double val) {
	double clampedVal = std::max(-1.0, std::min(1.0, val)); // clamp to [-1 | 1]
	// increase precision near contour by taking sqrt of distance
	return static_cast<glat::PNGImage::colorVal_t>(std::floor(clampedVal == 1.0 ? 255.0 : 127.0 + (std::sqrt(std::abs(clampedVal)) * 128.0 * (clampedVal < 0.0 ? -1.0 : 1.0)))); // map to [0 | 255]
}