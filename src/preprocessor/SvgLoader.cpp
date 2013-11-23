#include <glat/preprocessor/SvgLoader.h>
#include <iostream>
#include <math.h>

using namespace PREPROCESSOR;

SVGLoader::SVGLoader() {

}

std::string SVGLoader::RemoveCommas(std::string input) {
	//SVG allows for arbitrary whitespace and commas everywhere
	//sscanf ignores whitespace, so the only problem is commas
	for (unsigned int i = 0; i < input.length(); i++)
	if (input[i] == ',')
		input[i] = ' ';
	return input;
}

bool SVGLoader::parseGenericShapeProperty(tinyxml2::XMLAttribute* a, tinyxml2::XMLElement* elem) {
	std::string name(a->Name());

	if (name == "transform") {
		std::string s = a->Value();
		s = RemoveCommas(s);
		
		const char* ss = s.c_str();
		//parseTransform(ss, style().matrix);
		return true;
	}
	if (name == "color") {
		return true;
	}
	if (name == "fill") {
		std::string s = a->Value();
		const char* ss = s.c_str();

		//ss = parsePaintServer(ss, style(), style().fill);
		return true;
	}
	if (name == "stroke") {
		std::string s = a->Value();
		const char* ss = s.c_str();

		//ss = parsePaintServer(ss, style(), style().stroke);
		return true;
	}
	if (name == "stroke-width") {
		return true;
	}
	if (name == "stroke-dashoffset") {
		return true;
	}
	if (name == "stroke-dasharray") {
		return true;
	}
	if (name == "stroke-dashphase") {
		return true;
	}
	if (name == "style") {
		const std::string s = a->Value();
		const char* ss = s.c_str();

		//ss = parseStyle(ss, style());
		return true;
	}
	if (name == "class") {
		return true;
	}
	if (name == "opacity") {
		return true;
	}
	if (name == "fill-opacity") {
		return true;
	}
	if (name == "stop-color") {
		return true;
	}
	if (name == "stop-opacity") {
		return true;
	}
	if (name == "fill-rule") {
		return true;
	}
	if (name == "stroke-opacity") {
		return true;
	}
	if (name == "stroke-linecap") {
		return true;
	}
	if (name == "stroke-linejoin") {
		return true;
	}
	if (name == "stroke-miterlimit") {
		return true;
	}
	if (name == "clip-path") {
		return true;
	}
	if (name == "id") {
		std::string s = a->Value();
		//use_map[s] = elem;
		return true;
	}
	return false;
}

void SVGLoader::parseViewBox(const char *s) {
	int count = 0;
	float x, y, w, h;
	int rc = sscanf(s, " %f %f %f %f%n", &x, &y, &w, &h, &count);
	if (rc == 4 && count > 0) {
		m_viewbox.x = x;
		m_viewbox.y = y;
		m_viewbox.z = x + w;
		m_viewbox.w = y + h;
	}
}

void SVGLoader::parseSVG(tinyxml2::XMLElement* svg) {
	for (auto attr = svg->FirstAttribute(); attr; attr = attr->Next()) {
		std::string name(attr->Name());
		if (name == "width") m_width = atoi(attr->Value());
		else if (name == "height") m_height = atoi(attr->Value());
		else if (name == "viewbox") parseViewBox(attr->Value());
		else if (name == "preserveAspectRatio") m_preserve_aspect_ratio = attr->Value();
	}
}

void SVGLoader::loadSVG(const char* xmlFile) {
	tinyxml2::XMLDocument doc;
	if (!doc.LoadFile(xmlFile) == tinyxml2::XML_SUCCESS) 
		printf("File loading failed!");

	tinyxml2::XMLElement* svg = doc.FirstChildElement("svg");

	if (!svg)
		printf("svg-tag not found");

}