#ifndef GLAT_PNG_IMAGE_H
#define GLAT_PNG_IMAGE_H

#include <string>
#include <glm/glm.hpp>

#include <glat/Object.h>
#include <glow/Referenced.h>
#include <glow/ref_ptr.h>

namespace glat {
	class PNGImage : public glat::Object {
	public:
		typedef unsigned char colorVal_t;
		typedef struct image_t : public glow::Referenced{
			image_t(unsigned size);
			~image_t();
			colorVal_t *data;
		} image_t;

		// creates a blank Image
		PNGImage(unsigned width, unsigned height, unsigned numComponents = 4);

		// does not transforms given Image
		PNGImage(std::string distanceFieldFile);

		// transforms given src if dest does not exist
		PNGImage(std::string pngSrcFile, std::string destDistanceFieldFile);

		bool distanceTransformFromPNG(std::string fileName, unsigned minimalSideLength = 40);
		bool saveDistanceField(std::string fileName) const;
		bool loadImage(std::string fileName);

		void setImageValue(unsigned x, unsigned y, unsigned numComponent, colorVal_t value);
		colorVal_t getImageValue(unsigned x, unsigned y, unsigned numComponent) const;
		bool isColored(unsigned x, unsigned y) const;
		const glow::ref_ptr<image_t> getImage() const;

		unsigned getWidth() const;
		unsigned getHeight() const;


	protected:
		inline void createImage();

		glow::ref_ptr<image_t> m_image;
		unsigned m_width = 0, m_height = 0, m_imageComponents = 4;

	private:
		inline colorVal_t& imageValue(unsigned x, unsigned y, unsigned numComponent) const;

	};
}

#endif