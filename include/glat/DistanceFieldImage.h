#ifndef GLAT_DISTANCE_FIELD_IMAGE_H
#define GLAT_DISTANCE_FIELD_IMAGE_H

#include <string>
#include <glm/glm.hpp>

#include <glat/Object.h>

namespace glat {
	class DistanceFieldImage : public glat::Object {
	public:
		typedef char DistanceFieldValue;
		typedef DistanceFieldValue* DistanceField;

		// creates a blank Image
		DistanceFieldImage(unsigned width, unsigned height);

		// does not transforms given Image
		DistanceFieldImage(std::string distanceFieldFile);

		// transforms given src if dest does not exist
		DistanceFieldImage(std::string pngSrcFile, std::string destDistanceFieldFile);

		~DistanceFieldImage();

		bool generateFromPNG(std::string fileName, unsigned minimalSideLength = 40);
		bool loadImage(std::string fileName);
		bool saveImage(std::string fileName) const;
		void setDistance(unsigned x, unsigned y, DistanceFieldValue distance);

		unsigned getWidth() const;
		unsigned getHeight() const;

		DistanceFieldValue getDistance(unsigned x, unsigned y) const;
		const DistanceField getDistanceField() const;

	protected:
		inline void createImage();

		DistanceField m_image = nullptr;
		unsigned m_width = 0, m_height = 0;

	};
}

#endif