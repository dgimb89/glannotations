#ifndef GLAT_DISTANCE_FIELD_IMAGE_H
#define GLAT_DISTANCE_FIELD_IMAGE_H

#include <string>
#include <glm/glm.hpp>

namespace glat {
	class DistanceFieldImage {
	public:
		DistanceFieldImage(std::string fileName, unsigned width, unsigned height);
		~DistanceFieldImage();
		void loadImage(std::string fileName);
		void saveImage() const;
		void setDistance(unsigned x, unsigned y, float distance);
		float getDistance(unsigned x, unsigned y) const;

	protected:
		void createImage();
		void setWidth(unsigned width);
		void setHeight(unsigned height);

		typedef DistanceField glm::vec2;
		DistanceField* m_image;
		unsigned m_width = 0, m_height = 0;

	};
}

#endif