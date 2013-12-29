#ifndef GLAT_PNG_IMAGE_H
#define GLAT_PNG_IMAGE_H

#include <string>
#include <glm/glm.hpp>

#include <glat/Object.h>

namespace glat {
	class PNGImage : public glat::Object {
	public:
		typedef char image_t;
		typedef image_t* image_p;

		// creates a blank Image
		PNGImage(unsigned width, unsigned height, unsigned numComponents = 4);

		// does not transforms given Image
		PNGImage(std::string distanceFieldFile);

		// transforms given src if dest does not exist
		PNGImage(std::string pngSrcFile, std::string destDistanceFieldFile);

		~PNGImage();

		bool distanceTransformFromPNG(std::string fileName, unsigned minimalSideLength = 40);
		bool saveDistanceField(std::string fileName) const;
		bool loadImage(std::string fileName);

		void setImageValue(unsigned x, unsigned y, unsigned numComponent, image_t value);
		image_t getImageValue(unsigned x, unsigned y, unsigned numComponent) const;
		bool isColored(unsigned x, unsigned y);
		const image_p getImage() const;

		unsigned getWidth() const;
		unsigned getHeight() const;


	protected:
		inline void createImage();

		image_p m_image = nullptr;
		unsigned m_width = 0, m_height = 0, m_imageComponents = 4;

	private:
		inline image_t& imageValue(unsigned x, unsigned y, unsigned numComponent) const;

	};
}

#endif