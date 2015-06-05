#pragma once

#include <vector>
//#include <initializer_list>
#include <glm/glm.hpp>

#include <glannotations/DirtyFlagObject.h>
#include <glannotations/glannotations_api.h>

namespace glannotations {

    class GLANNOTATIONS_API BSpline : protected glannotations::DirtyFlagObject {
	public:
		//for 3D
		BSpline(std::vector<glm::vec3> ctrlPoints, std::vector<float> knotValues);
		BSpline(std::vector<glm::vec3> ctrlPoints, unsigned short degree);

		//for 2D within a given plane
		BSpline(std::vector<glm::vec2> ctrlPoints, std::vector<float> knotValues, glm::vec3 planeNormal, glm::vec3 firstControlPointOnTargetPlane, glm::vec3 lastControlPointOnTargetPlane);
		BSpline(std::vector<glm::vec2> ctrlPoints, unsigned short degree, glm::vec3 planeNormal, glm::vec3 firstControlPointOnTargetPlane, glm::vec3 lastControlPointOnTargetPlane);

		const std::vector<glm::vec3>& getControlPoints();
		const std::vector<float>& getKnotValues();
		unsigned short getSplineDegree();

		/*!
		*	\brief	Calculates a Curvepoint parametrized by t using de Boor's algorithm
		*	\param	t Curve parameter in range [0,1]
		*/
		glm::vec3 retrieveCurvepointAt(float t);

		/*!
		*	\brief	Calculates 2D-vector between two curvepoints specified by curve parameters t and nextT, not normalized
		*	\param	t Curve parameter for first curvepoint
		*	\param	nextT Curve parameter for second curvepoint
		*/
		glm::vec3 retrieveSecantVectorAt(float t, float nextT);

		// we want to use the dirty information just internally
        //virtual bool isDirty() const = delete;

		void setControlPoints(std::vector<glm::vec3> ctrlPoints);
		void setKnotValues(std::vector<float> knotValues);
		void calculateSplineDegree();

	protected:		
		void calculateUniformKnotValues();
		void project2DContropointsToPlane(std::vector<glm::vec2> ctrlPoints, glm::vec3 planeNormal, glm::vec3 firstControlPointOnTargetPlane, glm::vec3 lastControlPointOnTargetPlane);

		/**
		* Calculates values to fill the arcLength-container, which contains the arcLength at certain points along the curve,
		* used to approximate the overall arcLength and to position the glyphs evenly along the curve
		*/
		void calculateArcLengths();

		/*!
		*	\brief	returns point of the curve at position t
		*	\param	k the remaing searching depth, initial value == k
		*	\param	degree the degree of the spline
		*	\param	i position in knot vector
		*	\param	t position in curve-range[0,controlPoints.length]
		*	\param	knots defines the influence of the controlPoints
		*/
		glm::vec3 deBoor(int k, int degree, int i, float t, std::vector<float> knots);

		/*!
		*	\brief	returns t curve (or arcLength-) parameterized value in range [0,1]
		*	\param	u Parameterized value in range [0,1]
		*/
		float glannotations::BSpline::getTForU(float u);

		std::vector<glm::vec3> m_ctrlPoints;
		std::vector<float> m_knotValues;
		unsigned short m_degree;

		std::vector<float> m_arcLengths;
	};
}
