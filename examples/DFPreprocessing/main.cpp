#include <glat/DistanceFieldImage.h>

/** This example generates various Distance Fields from PNG, SVG, TrueTypeFonts and saves them into files
*/
int main(int argc, char* argv[]) {
	glat::DistanceFieldImage pngGenerated("test.png", "testdf.png");
	assert(!pngGenerated.isDirty());

	glat::DistanceFieldImage alreadyDF("testdf.png");
	assert(!alreadyDF.isDirty());
}