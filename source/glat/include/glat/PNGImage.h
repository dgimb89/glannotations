#pragma once

#include <glowbase/Referenced.h>
#include <glowbase/ref_ptr.h>
#include <string>
#include <glm/glm.hpp>

#include <glat/DirtyFlagObject.h>

namespace glat {
	class PNGImage : public glat::DirtyFlagObject {
	public:
		typedef unsigned char colorVal_t;
		typedef struct image_t : public glow::Referenced{
			image_t(size_t size);
			~image_t();
			colorVal_t *data;
		} image_t;

		// creates a blank Image
		PNGImage(size_t width, size_t height, unsigned short numComponents = 4, unsigned short bitdepth = 8);

		// load given png image
		PNGImage(std::string pngFileName);

		// transforms given src if dest does not exist
		PNGImage(std::string pngFileName, std::string destDistanceFieldFile);

		bool distanceTransformFromPNG(std::string pngFileName);
		void distanceTransform();

		void scaleToWidth(size_t scaledWidth);
		void scaleToHeight(size_t scaledHeight);
		void scale(double scaleFactor);

		bool saveDistanceField(std::string pngFileName) const;
		bool loadImage(std::string pngFileName);

		void setImageValue(size_t x, size_t y, unsigned short numComponent, colorVal_t value);

		// clamps for access out of image range
		inline colorVal_t getImageValue(signed long x, signed long y, unsigned short numComponent) const;
		bool isColored(size_t x, size_t y) const;
		const glow::ref_ptr<image_t> getImage() const;

		size_t getWidth() const;
		size_t getHeight() const;
		unsigned short getNumComponents() const;
		unsigned short getComponentBitdepth() const;
		inline size_t getRowStride() const;

		void replaceImageWith(glow::ref_ptr<glat::PNGImage> image);


	protected:
		inline void createImage();

		glow::ref_ptr<image_t> m_image;
		size_t m_width = 0, m_height = 0;
		unsigned short m_bitdepth = 0, m_channels = 4;

	private:
		inline colorVal_t& imageValue(size_t x, size_t y, unsigned short numComponent) const;

	};
}