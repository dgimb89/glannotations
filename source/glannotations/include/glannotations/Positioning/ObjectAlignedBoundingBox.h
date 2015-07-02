#pragma once

#include <glannotations/glannotations_api.h>
#include <glannotations/Positioning/SpaceObject.h>

#include <glm/glm.hpp>

namespace glannotations {
	/** \brief Represents a bounding box, that does't need to be axis aligned. Its edges stand perpendicular to each other.
	*/
	class GLANNOTATIONS_API ObjectAlignedBoundingBox : public SpaceObject {
	public:
		ObjectAlignedBoundingBox(glm::vec3 lowerLeftNear, glm::vec3 lowerLeftFar, glm::vec3 lowerRightNear, glm::vec3 upperLeftNear);
		ObjectAlignedBoundingBox(glm::vec3 positionLln, glm::vec3 width, glm::vec3 length, float height);

		virtual double getVolume() const override;
		virtual double distanceToCamera(const gloperate::Camera& cam) const override;
		virtual glm::vec3 getCentroid() const override;

		virtual bool isValid() const override;

	private:
		glm::vec3 urn() const;
		glm::vec3 ulf() const;
		glm::vec3 lrf() const;
		glm::vec3 urf() const;
		
		glm::vec3 m_lln; //lower left near
		glm::vec3 m_llf; //lower left far
		glm::vec3 m_lrn; //lower right near
		glm::vec3 m_uln; //upper left near

	};
}