#pragma once

#include <glannotations/Utility/BSpline.h>
#include <glannotations/glannotations_api.h>

#include <vector>
#include <glm/glm.hpp>

namespace glannotations {

    class GLANNOTATIONS_API BSpline2D : public glannotations::BSpline {
	public:
		BSpline2D(std::vector<glm::vec2> ctrlPoints, std::vector<float> knotValues);
		BSpline2D(std::vector<glm::vec2> ctrlPoints, unsigned short degree);

		void setControlPoints(std::vector<glm::vec2> ctrlPoints);

		glm::vec4 getBoundingRect();

	protected:
		//
	private:
		//
	};
}
