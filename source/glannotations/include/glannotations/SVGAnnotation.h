#pragma once

#include <glannotations/AbstractAnnotation.h>
#include <glannotations/glannotations_api.h>

namespace glannotations {
	class GLANNOTATIONS_API SVGAnnotation : public AbstractAnnotation {
	public:
		SVGAnnotation(const globjects::ref_ptr<glannotations::AbstractState>& initialState, const glannotations::RendererFactory& factory = RendererFactory());
		SVGAnnotation(const globjects::ref_ptr<glannotations::AbstractState>& initialState, std::string svgPathString, unsigned width, unsigned height, const glannotations::RendererFactory& factory = RendererFactory());

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
