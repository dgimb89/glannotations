#pragma once

#include <vector>
#include <initializer_list>
#include <glm/glm.hpp>

#include <glat/DirtyFlagObject.h>
#include <glat/glat_api.h>

namespace glat {
	class GLAT_API BSpline : public glat::DirtyFlagObject {
	public:
		BSpline(std::initializer_list<glm::vec3> ctrlPoints, unsigned short degree = 4);
		void append(std::initializer_list<glm::vec3> ctrlPoints);
		void append(glm::vec3 ctrlPoint);

		void setControlPoints(std::initializer_list<glm::vec3> ctrlPoints);
		const std::vector<glm::vec3>& getControlPoints();

		void setSplineDegree(unsigned short degree);
		unsigned short getSplineDegree();

		// we want to use the dirty information just internally
		virtual bool isDirty() = delete;

	protected:
		std::vector<glm::vec3> m_ctrlPoints;
		unsigned short m_degree;
	};
}