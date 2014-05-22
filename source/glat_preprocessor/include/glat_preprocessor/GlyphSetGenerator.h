#pragma once

#include <vector>
#include <string>
#include <glowbase/ref_ptr.h>

#include <glat/PNGImage.h>
#include <glat_preprocessor/glat_preprocessor_api.h>

namespace glat {
	namespace preprocessor {
		class GLAT_PREPROCESSOR_API GlyphSetGenerator
		{
		public:
			static void generateGlyphset(std::string fontFileName, unsigned numGlyphs, bool overrideExisting = false);
		protected:
			static glow::ref_ptr<PNGImage> generateGlyphImage(void* bitmap, unsigned marginLeft, int ascender, int descener, int bearingY);

			static inline int convertFontToPixelSize(int input);
			GlyphSetGenerator() {}
		};
	}
}
