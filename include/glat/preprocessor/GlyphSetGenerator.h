#ifndef GLAT_PREPROCESSOR_GLYPH_SET_GENERATOR_H
#define GLAT_PREPROCESSOR_GLYPH_SET_GENERATOR_H

#include <glat/PNGImage.h>
#include <glow/ref_ptr.h>
#include <ft2build.h>
#include FT_FREETYPE_H

namespace glat {
	namespace preprocessor {
		class GlyphSetGenerator
		{
		public:
			static bool generateGlyphset(std::string fontFileName, unsigned numGlyphs, bool extendExisting = true);
		protected:
			glow::ref_ptr<PNGImage> generateGlyphImage(FT_Bitmap* bitmap, FT_Int x, FT_Int y);
			GlyphSetGenerator() {}
		};
	}
}

#endif // !GLAT_PREPROCESSOR_GLYPH_SET_GENERATOR_H
