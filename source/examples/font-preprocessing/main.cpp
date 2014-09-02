#include <glat/PNGImage.h>
#include <glat-preprocessor/GlyphSetGenerator.h>

/** This example generates various Distance Fields from PNG, SVG, TrueTypeFonts and saves them into files
*/
int main(int argc, char* argv[]) {

	// Distance Transform 
	{
		glat::PNGImage pngExample("glat.png");
		assert(!pngExample.isDirty());

		pngExample.distanceTransform();
		assert(pngExample.isDirty());
		assert(pngExample.getNumComponents() == 1);
		assert(pngExample.getComponentBitdepth() == 8);

		pngExample.scaleToHeight(128);
		assert(pngExample.getHeight() == 128);

		pngExample.saveDistanceField("glat_df.png");
		assert(!pngExample.isDirty());

		glat::PNGImage pngDFExample("glat_df.png");
		assert(!pngExample.isDirty());
		assert(pngDFExample.getHeight() == 128);
	}

	// Glyphset Generation
	{
	glat::preprocessor::GlyphSetGenerator::generateGlyphset("calibri.ttf", 90, true);
	}
	
}