#ifndef GLAT_PREPROCESSOR_GLYPH_SET_GENERATOR_H
#define GLAT_PREPROCESSOR_GLYPH_SET_GENERATOR_H

#include <glat/PNGImage.h>
#include <glow/ref_ptr.h>

namespace glat {
	namespace preprocessor {
		class GlyphSetGenerator
		{
		public:
			static bool generateGlyphset(std::string fontFileName, unsigned numGlyphs, bool extendExisting = true);
		protected:
			static glow::ref_ptr<PNGImage> generateGlyphImage(void* bitmap, unsigned marginLeft, int ascender, int descener, int bearingY);
			static inline int convertFontToPixelSize(int input);
			GlyphSetGenerator() {}
		};
	}
}

#endif // !GLAT_PREPROCESSOR_GLYPH_SET_GENERATOR_H
