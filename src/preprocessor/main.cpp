#include <glat/preprocessor/SvgLoader.h>

using namespace PREPROCESSOR;

int main(int argc, char* argv[]) {
	SVGLoader* loader = new SVGLoader();
	loader->loadSVG("test.svg");
	return 0;
}