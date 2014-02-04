#ifndef GLAT_PREPROCESSOR_GLYPH_SET_GENERATOR_H
#define GLAT_PREPROCESSOR_GLYPH_SET_GENERATOR_H

#include <glow/ref_ptr.h>
#include <glat/PNGImage.h>
#include <vector>
#include <string>

namespace glat {
	namespace preprocessor {
		class GlyphSetGenerator
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
#endif // !GLAT_PREPROCESSOR_GLYPH_SET_GENERATOR_H
