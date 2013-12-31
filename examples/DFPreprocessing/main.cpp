#include <glat/PNGImage.h>

/** This example generates various Distance Fields from PNG, SVG, TrueTypeFonts and saves them into files
*/
int main(int argc, char* argv[]) {
	glat::PNGImage pngGenerated("test.png", "test_df.png");
	assert(!pngGenerated.isDirty());

	/*glat::PNGImage alreadyDF("testdf.png");
	assert(!alreadyDF.isDirty());*/
}