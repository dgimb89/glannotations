#pragma once

#include <vector>
#include <string>
#include <globjects/base/ref_ptr.h>

#include <glannotations/PNGImage.h>
#include <glannotations-preprocessor/glat-preprocessor_api.h>

namespace glat {
	namespace preprocessor {
		class GLAT_PREPROCESSOR_API GlyphSetGenerator
		{
		public:
			static void generateGlyphset(std::string fontFileName, unsigned numGlyphs, bool overrideExisting = false);
		protected:
			static globjects::ref_ptr<PNGImage> generateGlyphImage(void* bitmap, unsigned marginLeft, int ascender, int descener, int bearingY);

			static int convertFontToPixelSize(int input);
			GlyphSetGenerator() {}
		};
	}
}