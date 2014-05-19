#pragma once

#include <glat/AbstractAnnotation.h>

namespace glat {
	class SVGAnnotation : public AbstractAnnotation {
	public:
		SVGAnnotation(glat::AbstractState* initialState, const glat::RendererFactory& factory = RendererFactory());
		SVGAnnotation(glat::AbstractState* initialState, std::string svgPathString, unsigned width, unsigned height, const glat::RendererFactory& factory = RendererFactory());

		inline void setPathString(std::string svgPathString);
		inline void setWidth(unsigned width);
		inline void setHeight(unsigned height);

		const std::string& getPathString() const;
		unsigned getHeight() const;
		unsigned getWidth() const;
	protected:
		std::string m_svgPathString;
		unsigned m_width, m_height;
	};
}