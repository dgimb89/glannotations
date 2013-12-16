#ifndef GLAT_SVG_ANNOTATION_H
#define GLAT_SVG_ANNOTATION_H

#include <glat/AbstractAnnotation.h>

namespace glat {
	class SVGAnnotation : public AbstractAnnotation {
	public:
		SVGAnnotation(glat::AbstractState* initialState, const glat::RendererFactory& factory = RendererFactory());
		SVGAnnotation(glat::AbstractState* initialState, std::string svgPathString, const glat::RendererFactory& factory = RendererFactory());
		void setPathString(std::string svgPathString);
		const std::string& getPathString();
	protected:
		std::string m_svgPathString;
	};
}

#endif