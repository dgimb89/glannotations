#pragma once

#include <glannotations/Utility/BSpline.h>
#include <glannotations/glannotations_api.h>

#include <vector>
#include <glm/glm.hpp>

namespace glannotations {

    class GLANNOTATIONS_API BSpline3D : public glannotations::BSpline {
	public:
		BSpline3D(std::vector<glm::vec3> ctrlPoints, std::vector<float> knotValues);
		BSpline3D(std::vector<glm::vec3> ctrlPoints, unsigned short degree);

		void setControlPoints(std::vector<glm::vec3> ctrlPoints);
		
	protected:
		//
	private:
		//
	};
}
