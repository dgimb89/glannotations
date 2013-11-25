#include <glat/preprocessor/DistanceFieldGenerator.h>

const glat::preprocessor::DistanceFieldGenerator::Image& distanceTransform() {

	const int hresW = picture.getWidth();
	const int hresH = picture.getHeight();

	//	TODO: Breite und Höhe des Distanzfeldes berechnen:
	const int lresW = std::max(minimalSideLength * hresW / hresH, minimalSideLength);
	const int lresH = std::max(minimalSideLength * hresH / hresW, minimalSideLength);

	const int scale = hresW / lresW;

	Picture * distanceField = new Picture(lresW, lresH);
	int middleX, middleY;

	for (int actualX = 0; actualX < lresW; actualX++) {
		for (int actualY = 0; actualY < lresH; actualY++) {
			middleX = scale * actualX;
			middleY = scale * actualY;
			bool isBlack = picture.getColor(middleX, middleY).isBlack();	// distance sign
			float distance = sqrtf(powf(kernelSize / 2.0f, 2) + powf(kernelSize / 2.0f, 2));

			for (int kernelX = std::max(middleX - (int)(kernelSize / 2.0f), 0); kernelX <= std::min(middleX + (int)(kernelSize / 2.0f), hresW - 1); kernelX++) {
				for (int kernelY = std::max(middleY - (int)(kernelSize / 2.0f), 0); kernelY <= std::min(middleY + (int)(kernelSize / 2.0f), hresH - 1); kernelY++) {
					if (isBlack != picture.getColor(kernelX, kernelY).isBlack()) {
						distance = std::min(distance, std::sqrtf(std::powf(kernelX - middleX, 2.0f) + std::powf(kernelY - middleY, 2.0f)));
					}
				}
			}
			distance /= sqrtf(powf(kernelSize / 2.0f, 2) + powf(kernelSize / 2.0f, 2));
			distance = 1 - distance;
			distance /= 2.0f;
			if (!isBlack)
				distance += 0.3f;

			distanceField->setColor(actualX, actualY, ColorRGBA(distance, distance, distance, 1));
		}
	}

	return distanceField;
}

bool glat::preprocessor::DistanceFieldGenerator::isDirty() {
	return m_dirty;
}

bool glat::preprocessor::DistanceFieldGenerator::setDirty(bool dirty) {
	m_dirty = dirty;
}
