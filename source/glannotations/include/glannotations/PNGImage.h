#pragma once

#include <globjects/base/Referenced.h>
#include <globjects/base/ref_ptr.h>
#include <string>
#include <glm/glm.hpp>

#include <glannotations/DirtyFlagObject.h>
#include <glannotations/glannotations_api.h>


namespace glannotations {

	typedef unsigned char colorVal_t;
	typedef struct GLANNOTATIONS_API image_t : public globjects::Referenced {
		image_t(size_t size);
		~image_t();
		colorVal_t *data;
	} image_t;


	template class GLANNOTATIONS_API std::basic_string < char, std::char_traits<char>, std::allocator<char> > ;
	template class GLANNOTATIONS_API globjects::ref_ptr < image_t > ;

	class GLANNOTATIONS_API PNGImage : public glannotations::DirtyFlagObject {
	public:

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
		colorVal_t getImageValue(size_t x, size_t y, unsigned short numComponent) const;
		bool isColored(size_t x, size_t y) const;
		const globjects::ref_ptr<glannotations::image_t> getImage() const;

		size_t getWidth() const;
		size_t getHeight() const;
		unsigned short getNumComponents() const;
		unsigned short getComponentBitdepth() const;
		size_t getRowStride() const;

		void replaceImageWith(globjects::ref_ptr<glannotations::PNGImage> image);


	protected:
		void createImage();

		size_t m_width = 0, m_height = 0;
		unsigned short m_bitdepth = 0, m_channels = 4;

	private:
		globjects::ref_ptr<image_t> m_image;
		colorVal_t& imageValue(size_t x, size_t y, unsigned short numComponent) const;

	};
}