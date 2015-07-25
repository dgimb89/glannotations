#pragma once

#include <glannotations/glannotations_api.h>
#include <glannotations/Positioning/SpaceObject.h>

#include <glm/glm.hpp>

namespace glannotations {
	/** \brief Represents a bounding box, that does't need to be axis aligned. Its edges stand perpendicular to each other.
	*/
	class GLANNOTATIONS_API ObjectAlignedBoundingBox : public glannotations::SpaceObject {
	public:
		ObjectAlignedBoundingBox(glm::vec3 llf, glm::vec3 llb, glm::vec3 lrf, glm::vec3 ulf);
		ObjectAlignedBoundingBox(glm::vec3 positionLln, glm::vec3 width, glm::vec3 length, float height);

		virtual double getVolume() const override;
		virtual double distanceToCamera(const gloperate::Camera& cam) const override;
		virtual glm::vec3 getCentroid() const override;
		virtual std::array<glm::vec3, 4> getBoundingBox() override;

		virtual bool isValid() const override;

	private:
		glm::vec3 urf() const;
		glm::vec3 ulb() const;
		glm::vec3 lrb() const;
		glm::vec3 urb() const;

		glm::vec3 m_llf; //lower left front
		glm::vec3 m_llb; //lower left back
		glm::vec3 m_lrf; //lower right front
		glm::vec3 m_ulf; //upper left front

	};
}