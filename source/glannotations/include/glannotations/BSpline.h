#pragma once

#include <vector>
#include <initializer_list>
#include <glm/glm.hpp>
#include <glm/gtx/rotate_vector.hpp>

#include <glannotations/DirtyFlagObject.h>
#include <glannotations/glannotations_api.h>

namespace glannotations {

	enum GlyphOrientationOnSpline{ IN_SAME_PLANE, ORTHOGONAL_TO_PLANE, CUSTOM_SECOND_PATH };

    class GLANNOTATIONS_API BSpline : protected glannotations::DirtyFlagObject {
	public:
		BSpline(std::initializer_list<glm::vec3> ctrlPoints, std::initializer_list<float> knotValues);
		BSpline(std::initializer_list<glm::vec3> ctrlPoints, unsigned short degree);

		const std::vector<glm::vec3>& getControlPoints();
		const std::vector<float>& getKnotValues();
		unsigned short getSplineDegree();

		/**
		* calculates a Curvepoint parametrized by t using de Boor's algorithm
		* t Curve parameter in range [0,1]
		*/
		glm::vec3 retrieveCurvepointAt(float t);

		/**
		* Calculates 2D-vector between two curvepoints specified by curve parameters t and nextT, not normalized
		* t Curve parameter for first curvepoint
		* nextT Curve parameter for second curvepoint
		*/
		glm::vec3 retrieveSecantVectorAt(float t, float nextT);

		/**
		* Calculates orthogonal vector to the secantvector between two curvepoints specified by curve parameters t and nextT
		* t Curve parameter for first curvepoint
		* nextT Curve parameter for second curvepoint
		*/
		glm::vec3 retrieveNormalizedOrthogonalVectorAt(float t, float nextT, enum GlyphOrientationOnSpline orientation);

		// we want to use the dirty information just internally
        //virtual bool isDirty() const = delete;

	protected:
		void setControlPoints(std::initializer_list<glm::vec3> ctrlPoints);
		void setKnotValues(std::initializer_list<float> knotValues);
		void calculateSplineDegree();
		void calculateUniformKnotValues();

		/**
		* Calculates values to fill the arcLength-container, which contains the arcLength at certain points along the curve,
		* used to approximate the overall arcLength and to position the glyphs evenly along the curve
		*/
		void calculateArcLengths();

		/**
		* returns point of the curve at position t
		* k the remaing searching depth, initial value == k
		* degree the degree of the spline
		* i position in knot vector
		* t position in curve-range[0,controlPoints.length]
		* knots defines the influence of the controlPoints
		*/
		glm::vec3 deBoor(int k, int degree, int i, float t, std::vector<float> knots);

		std::vector<glm::vec3> m_ctrlPoints;
		std::vector<float> m_knotValues;
		unsigned short m_degree;

		std::vector<float> m_arcLengths;
	};
}
