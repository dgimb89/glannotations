#pragma once
#include <tinyxml2.h>
#include <string>
namespace glat {
	namespace preprocessor {

		typedef struct float4 {
			float x;
			float y;
			float z;
			float w;
		} float4;

		class SVGLoader {
		public:
			SVGLoader();
			void loadSVG(const char* xmlFile);

		private:
			void parseSVG(tinyxml2::XMLElement* svg);
			void parseViewBox(const char *s);
			bool parseGenericShapeProperty(tinyxml2::XMLAttribute* a, tinyxml2::XMLElement* elem);
			std::string RemoveCommas(std::string input);


			int m_width;
			int m_height;
			std::string m_preserve_aspect_ratio;
			float4 m_viewbox;
			float m_matrix[9];
		};
	}
}