#pragma once

#include <vector>
#include <initializer_list>
#include <glm/glm.hpp>

#include <glannotations/DirtyFlagObject.h>
#include <glannotations/glannotations_api.h>

namespace glannotations {
    class GLANNOTATIONS_API BSpline : protected glannotations::DirtyFlagObject {
	public:
		BSpline(std::initializer_list<glm::vec3> ctrlPoints, std::initializer_list<float> knotValues);
		BSpline(std::initializer_list<glm::vec3> ctrlPoints, unsigned short degree);

		const std::vector<glm::vec3>& getControlPoints();
		const std::vector<float>& getKnotValues();
		unsigned short getSplineDegree();

		// we want to use the dirty information just internally
        //virtual bool isDirty() const = delete;

	protected:
		void setControlPoints(std::initializer_list<glm::vec3> ctrlPoints);
		void setKnotValues(std::initializer_list<float> knotValues);
		void calculateSplineDegree();
		void calculateUniformKnotValues();

		std::vector<glm::vec3> m_ctrlPoints;
		std::vector<float> m_knotValues;
		unsigned short m_degree;
	};
}
