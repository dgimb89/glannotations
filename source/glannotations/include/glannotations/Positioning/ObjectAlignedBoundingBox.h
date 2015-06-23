#pragma once

#include <glannotations/glannotations_api.h>
#include <glannotations/Positioning/SpaceObject.h>

namespace glannotations {
	class GLANNOTATIONS_API ObjectAlignedBoundingBox : public SpaceObject {
	public:
		ObjectAlignedBoundingBox(/* todo: find good geometric rep. for bounding box */);

		virtual double getVolume() const override;
		virtual double distanceToCamera(const gloperate::Camera& cam) const override;
		virtual glm::vec3 getCentroid() const override;
	};
}