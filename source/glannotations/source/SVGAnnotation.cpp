#include <glannotations/SVGAnnotation.h>

glannotations::SVGAnnotation::SVGAnnotation(const globjects::ref_ptr<glannotations::AbstractState>& initialState, const glannotations::RendererFactory& factory /*= RendererFactory()*/)
: AbstractAnnotation(initialState){
	m_renderer = factory.createRenderer(*this);
}

glannotations::SVGAnnotation::SVGAnnotation(const globjects::ref_ptr<glannotations::AbstractState>& initialState, std::string svgPathString, unsigned width, unsigned height, const glannotations::RendererFactory& factory /*= RendererFactory()*/)
	: SVGAnnotation(initialState, factory) {
	setPathString(svgPathString);
	setWidth(width);
	setHeight(height);
}

void glannotations::SVGAnnotation::setPathString(std::string svgPathString) {
	setDirty(true);
	m_svgPathString = svgPathString;
}

const std::string& glannotations::SVGAnnotation::getPathString() const {
	return m_svgPathString;
}

unsigned glannotations::SVGAnnotation::getHeight() const {
	return m_height;
}

unsigned glannotations::SVGAnnotation::getWidth() const {
	return m_width;
}

void glannotations::SVGAnnotation::setWidth(unsigned width) {
	setDirty(true);
	m_width = width;
}

void glannotations::SVGAnnotation::setHeight(unsigned height) {
	setDirty(true);
	m_height = height;
}
