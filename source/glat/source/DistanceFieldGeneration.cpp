#include <glat/DistanceFieldGeneration.h>
#include <algorithm>

// use fixed kernel distances - thanks pythagoras
const double kernel[] = {	0.0, 	1.0, 					2.0,
							1.0, 	std::sqrt(1 + 1), 		std::sqrt(4 + 1),
							2.0,	std::sqrt(4 + 1),		std::sqrt(8) };

#define SWAPIFLOWER(y,x,swapVal) if((x) >= 0 && (x) < original.getWidth() && (y) >= 0 && (y) < original.getHeight()) \
	distances[(y)*original.getWidth() + (x)] = (original.isColored(x, y) ? -1.0 : 1.0) * std::min(std::abs(distances[(y)*original.getWidth() + (x)]), std::abs(swapVal))

bool glat::DistanceFieldGeneration::detail::selfColoredNeighborsNot(const glat::PNGImage& original, unsigned x, unsigned y) {
	return original.isColored(x, y) && 
		((x > 0 ? !original.isColored(x - 1, y) : true) || (y > 0 ? !original.isColored(x, y - 1) : true) || 
		(x < original.getWidth() ? !original.isColored(x + 1, y) : true) || (y < original.getHeight() ? !original.isColored(x, y + 1) : true));
}

glo::ref_ptr<glat::PNGImage> glat::DistanceFieldGeneration::bilinearResize(const glat::PNGImage& inImage, unsigned scaledWidth, unsigned scaledHeight) {
	glo::ref_ptr<glat::PNGImage> scaledResult = new glat::PNGImage(scaledWidth, scaledHeight, inImage.getNumComponents());

	const double tx = double(inImage.getWidth()) / scaledWidth;
	const double ty = double(inImage.getHeight()) / scaledHeight;
	const unsigned channels = inImage.getNumComponents();

	for (unsigned y = 0; y < scaledHeight; ++y){
		for (unsigned x = 0; x < scaledWidth; ++x) {
			for (unsigned c = 0; c < channels; ++c) {
				double xPart = (x * tx) - std::floor(x * tx);
				double yPart = (y * ty) - std::floor(y * ty);
				double R_1 = ((1.0 - xPart) * inImage.getImageValue(std::floor(x * tx), std::floor(y * ty), c)) + (xPart * inImage.getImageValue(std::ceil(x * tx), std::floor(y * ty), c));
				double R_2 = ((1.0 - xPart) * inImage.getImageValue(std::floor(x * tx), std::ceil(y * ty), c)) + (xPart * inImage.getImageValue(std::ceil(x * tx), std::ceil(y * ty), c));

				scaledResult->setImageValue(x, y, c, static_cast<glat::PNGImage::colorVal_t>((1.0 - yPart) * R_1 + (yPart * R_2)));
			}
		}
	}

	return scaledResult;
}

inline double cubicInterpolate(double p, double cur, double n1, double n2, double frac) {
	return cur + 0.5 * frac *(n1 - p + frac*(2.0*p - 5.0*cur + 4.0*n1 - n2 + frac*(3.0*(cur - n1) + n2 - p)));
}

glo::ref_ptr<glat::PNGImage> glat::DistanceFieldGeneration::bicubicResize(const glat::PNGImage& inImage, unsigned scaledWidth, unsigned scaledHeight) {
	glo::ref_ptr<glat::PNGImage> scaledResult = new glat::PNGImage(scaledWidth, scaledHeight, inImage.getNumComponents());

	const double tx = double(inImage.getWidth()) / scaledWidth;
	const double ty = double(inImage.getHeight()) / scaledHeight;
	const unsigned channels = inImage.getNumComponents();

	for (unsigned y = 0; y < scaledHeight; ++y){
		for (unsigned x = 0; x < scaledWidth; ++x) {
			for (unsigned c = 0; c < channels; ++c) {
				double yFrac = ty * y - std::floor(ty * y);
				signed long lowestX = static_cast<signed long>(std::floor(tx * x) - 1.0);
				signed long lowestY = static_cast<signed long>(std::floor(ty * y) - 1.0);
				double p0 = cubicInterpolate(inImage.getImageValue(lowestX, lowestY, c), inImage.getImageValue(lowestX, lowestY + 1, c), inImage.getImageValue(lowestX, lowestY + 2, c), inImage.getImageValue(lowestX, lowestY + 3, c), yFrac);
				double p1 = cubicInterpolate(inImage.getImageValue(lowestX + 1, lowestY, c), inImage.getImageValue(lowestX + 1, lowestY + 1, c), inImage.getImageValue(lowestX + 1, lowestY + 2, c), inImage.getImageValue(lowestX + 1, lowestY + 3, c), yFrac);
				double p2 = cubicInterpolate(inImage.getImageValue(lowestX + 2, lowestY, c), inImage.getImageValue(lowestX + 2, lowestY + 1, c), inImage.getImageValue(lowestX + 2, lowestY + 2, c), inImage.getImageValue(lowestX + 2, lowestY + 3, c), yFrac);
				double p3 = cubicInterpolate(inImage.getImageValue(lowestX + 3, lowestY, c), inImage.getImageValue(lowestX + 3, lowestY + 1, c), inImage.getImageValue(lowestX + 3, lowestY + 2, c), inImage.getImageValue(lowestX + 3, lowestY + 3, c), yFrac);
				double result = cubicInterpolate(p0, p1, p2, p3, (tx * x) - std::floor(tx * x));
				scaledResult->setImageValue(x, y, c, static_cast<glat::PNGImage::colorVal_t>(std::round(result)));
			}
		}
	}
	return scaledResult;
}

glo::ref_ptr<glat::PNGImage> glat::DistanceFieldGeneration::distanceTransform(const glat::PNGImage& original) {
	double* distances = new double[original.getHeight() * original.getWidth()];
	std::fill_n(distances, original.getHeight() * original.getWidth(), std::numeric_limits<double>::infinity());
	// top-left to bottom-right
	for (unsigned y = 0; y < original.getHeight(); ++y) {
		for (unsigned x = 0; x < original.getWidth(); ++x) {
			if (detail::selfColoredNeighborsNot(original, x, y)) {
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
			// apply kernel
			double currentVal = std::abs(distances[(y - 1) * original.getWidth() + (x - 1)]);
			if (currentVal == INFINITY) continue;
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

	glo::ref_ptr<glat::PNGImage> distanceField = new glat::PNGImage(original.getWidth(), original.getHeight(), 1);
	for (unsigned y = 0; y < original.getHeight(); ++y) {
		for (unsigned x = 0; x < original.getWidth(); ++x) {
			distanceField->setImageValue(x, y, 0, colorValueFromFloat(distances[y * original.getWidth() + x]));
		}
	}
	return distanceField;
}

glat::PNGImage::colorVal_t glat::DistanceFieldGeneration::colorValueFromFloat(double val) {
	double clampedVal = std::max(-127.0, std::min(128.0, ((val < 0.0)? -1.0 : 1.0) *  std::sqrt(std::abs(val)) * 128)); // clamp to [-127 | 128]
	return static_cast<glat::PNGImage::colorVal_t>(127.0 + clampedVal); // map to [0 | 255]
}