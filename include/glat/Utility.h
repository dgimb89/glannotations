#pragma once

#include <vector>
#include <iterator>
#include <glm/glm.hpp>
#include <limits>

namespace glat {
	namespace Utility {
		template <typename genType>
		static void computeNextNearestPoint(unsigned numPoints, const genType& reference, std::vector<genType>& points, std::vector<genType>& writeBack) {
			float minDistance = std::numeric_limits<float>::infinity();
			unsigned minDistancePoint = 0;

			for (unsigned index = 0; index < points.size(); ++index) {
				float curDistance = glm::distance(points.at(index), reference);
				if (curDistance < minDistance) {
					minDistancePoint = index;
					minDistance = curDistance;
				}
			}
			writeBack.push_back(points.at(minDistancePoint));
			points.erase(points.begin() + minDistancePoint);
			if (numPoints > 1) {
				computeNextNearestPoint(numPoints - 1, reference, points, writeBack);
			}
		}

		template <typename genType>
		static std::vector<genType> getNearestPoints(unsigned numPoints, const genType& reference, std::vector<genType> points) {
			std::vector<genType> result;
			computeNextNearestPoint(numPoints, reference, points, result);
			return result;
		}
	}
}