#pragma once

#include <glannotations/Utility/BSpline.h>
#include <glannotations/glannotations_api.h>

#include <vector>
#include <glm/glm.hpp>

namespace glannotations {

    class GLANNOTATIONS_API BSpline2D : public glannotations::BSpline {
	public:
		BSpline2D(glm::vec3 planeAxisRight, glm::vec3 planeAxisUp, std::vector<glm::vec2> ctrlPoints, std::vector<float> knotValues);
		BSpline2D(glm::vec3 planeAxisRight, glm::vec3 planeAxisUp, std::vector<glm::vec2> ctrlPoints, unsigned short degree);

		glm::vec3 getPlaneNormal() const;
		glm::vec3 getPlaneRight() const;
		glm::vec3 getPlaneUp() const;

		void setControlPoints(std::vector<glm::vec2> ctrlPoints);
		void setPlane(glm::vec3 planeAxisRight, glm::vec3 planeAxisUp);

		bool isInSamePlane(std::shared_ptr<BSpline2D> compareSpline) const;
		glm::vec4 getBoundingRect();
		glm::mat4 retrieveTransformation();

	protected:
		//
	private:
		/*!
		*	\brief	recalculates the ctrlPoints in 3D space, using plane and 2D-ctrlPoints
		*			is called automatically when 2D-ctrlPoints or plane are changed
		*/
		void updateControlPoints3D();

		//to specify the plane which contains the spline
		glm::vec3 m_right;				//normalized
		glm::vec3 m_up;						//normalized
		float m_width;						//in 3D world space, used for scaling from 2D plane
		float m_height;						//in 3D world space, used for scaling from 2D plane

		bool m_transformationDirty;
		glm::mat4 m_transformation;
		
		std::vector<glm::vec3> m_transformedCtrlPoints;
	};
}
