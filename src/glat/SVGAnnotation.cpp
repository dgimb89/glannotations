#include <glat/SVGAnnotation.h>

glat::SVGAnnotation::SVGAnnotation(glat::AbstractState* initialState, const glat::RendererFactory& factory /*= RendererFactory()*/)
: AbstractAnnotation(initialState){
	m_renderer = factory.createRenderer(*this);
}

glat::SVGAnnotation::SVGAnnotation(glat::AbstractState* initialState, std::string svgPathString, const glat::RendererFactory& factory /*= RendererFactory()*/)
	: SVGAnnotation(initialState, factory) {
	setPathString(svgPathString);
}

void glat::SVGAnnotation::setPathString(std::string svgPathString) {
	setDirty(true);
	m_svgPathString = svgPathString;
}

const std::string& glat::SVGAnnotation::getPathString() {
	return m_svgPathString;
}