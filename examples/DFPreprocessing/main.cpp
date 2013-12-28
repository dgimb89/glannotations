#include <glat/DistanceFieldImage.h>

/** This example generates various Distance Fields from PNG, SVG, TrueTypeFonts and saves them into files
*/
int main(int argc, char* argv[]) {
	glat::DistanceFieldImage pngGenerated("test.png", "test.png.df");
	assert(!pngGenerated.isDirty());

	glat::DistanceFieldImage alreadyDF("test_df.png");
	assert(!alreadyDF.isDirty());
}