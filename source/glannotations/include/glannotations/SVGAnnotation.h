#pragma once

#include <glannotations/AbstractAnnotation.h>
#include <glannotations/glannotations_api.h>

namespace glat {
	class GLANNOTATIONS_API SVGAnnotation : public AbstractAnnotation {
	public:
		SVGAnnotation(const globjects::ref_ptr<glat::AbstractState>& initialState, const glat::RendererFactory& factory = RendererFactory());
		SVGAnnotation(const globjects::ref_ptr<glat::AbstractState>& initialState, std::string svgPathString, unsigned width, unsigned height, const glat::RendererFactory& factory = RendererFactory());

		void setPathString(std::string svgPathString);
		void setWidth(unsigned width);
		void setHeight(unsigned height);

		const std::string& getPathString() const;
		unsigned getHeight() const;
		unsigned getWidth() const;
	protected:
		std::string m_svgPathString;
		unsigned m_width, m_height;
	};
}
