#pragma once

#include <vector>
#include <string>
#include <globjects/base/ref_ptr.h>

#include <glannotations/PNGImage.h>
#include <glannotations-preprocessor/glannotations-preprocessor_api.h>

namespace glannotations {
	namespace preprocessor {
		class GLANNOTATIONS_PREPROCESSOR_API GlyphSetGenerator
		{
		public:
			static void generateGlyphset(std::string fontFileName, unsigned numGlyphs, bool overrideExisting = false);
		protected:
			static globjects::ref_ptr<PNGImage> generateGlyphImage(void* bitmap, unsigned marginLeft, int ascender, int descener, int bearingY);

			static double convertFontToPixelSize(int input);
			GlyphSetGenerator() {}
		};
	}
}
