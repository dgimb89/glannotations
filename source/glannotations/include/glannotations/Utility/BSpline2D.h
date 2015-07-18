#pragma once

#include <glannotations/Utility/BSpline.h>
#include <glannotations/glannotations_api.h>

#include <vector>
#include <glm/glm.hpp>

namespace glannotations {

    class GLANNOTATIONS_API BSpline2D : public glannotations::BSpline {
	public:
		BSpline2D(glm::vec3 planeAxisDirection, glm::vec3 planeAxisUp, std::vector<glm::vec2> ctrlPoints, std::vector<float> knotValues);
		BSpline2D(glm::vec3 planeAxisDirection, glm::vec3 planeAxisUp, std::vector<glm::vec2> ctrlPoints, unsigned short degree);

		const std::vector<glm::vec2>& getControlPoints2D() const;
		const glm::vec3& getPlaneNormal() const;

		void setControlPoints(std::vector<glm::vec2> ctrlPoints);
		void setPlane(glm::vec3 planeAxisDirection, glm::vec3 planeAxisUp);

		bool isInSamePlane(std::shared_ptr<BSpline2D> compareSpline) const;

	protected:
		//
	private:
		/*!
		*	\brief	recalculates the ctrlPoints in 3D space, using plane and 2D-ctrlPoints
		*			is called automatically when 2D-ctrlPoints or plane are changed
		*/
		void updateControlPoints3D();

		//to specify the plane which contains the spline
		glm::vec3 m_direction;				//normalized
		glm::vec3 m_up;						//normalized
		
		std::vector<glm::vec2> m_ctrlPoints2D;
	};
}
