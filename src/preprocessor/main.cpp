#include <glat/preprocessor/SvgLoader.h>

using namespace glat::preprocessor;

int main(int argc, char* argv[]) {
	SVGLoader* loader = new SVGLoader();
	loader->loadSVG("test.svg");
	return 0;
}