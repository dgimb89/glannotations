#pragma once

#include <glannotations/States/AbstractState.h>

template<typename T>
void glannotations::AbstractState::cropExtends(T& ll, T& lr, T& ur, glm::vec2 sourceExtends) {
	// calculating width / height to fit in given ll,lr,ur extends and keep aspect ratio
	float height = glm::distance(lr, ur);
	float width = glm::distance(ll, lr);
	if (hasMaximumHeight() && height > getMaximumHeight()) {
		// shrink height
		height = getMaximumHeight();
	}

	// assuring aspect ratio
	if (getSourceKeepAspectRatio()) {
		float aspectRatio = sourceExtends.x / sourceExtends.y;
		if ((width / height) > aspectRatio) {
			// shrink width
			width = aspectRatio * height;
		}
		else {
			// shrink height
			height = width / aspectRatio;
		}
	}

	// recalculating center according to width/height + anchor settings
	T widthSpan, heightSpan;
	widthSpan = glm::normalize(lr - ll) * 0.5f * width;
	heightSpan = glm::normalize(ur - lr) * 0.5f * height;

	T center;
	switch (getHorizontalAnchor()) {
	case Anchor::LEFT:
		center = ll + widthSpan;
		break;
	case Anchor::CENTER:
		center = (ll + lr) / 2.f;
		break;
	case Anchor::RIGHT:
		center = lr - widthSpan;
		break;
	}

	switch (getVerticalAnchor()) {
	case Anchor::BOTTOM:
		center += heightSpan;
		break;
	case Anchor::MIDDLE:
		center += (ur - lr) / 2.f;
		break;
	case Anchor::TOP:
		center += (ur - lr) - heightSpan;
		break;
	}

	// settings new extends based on new center
	ll = center - heightSpan - widthSpan;
	lr = center - heightSpan + widthSpan;
	ur = center + heightSpan + widthSpan;
}
