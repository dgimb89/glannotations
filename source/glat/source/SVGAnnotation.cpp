#include <glat/SVGAnnotation.h>

glat::SVGAnnotation::SVGAnnotation(const glo::ref_ptr<glat::AbstractState>& initialState, const glat::RendererFactory& factory /*= RendererFactory()*/)
: AbstractAnnotation(initialState){
	m_renderer = factory.createRenderer(*this);
}

glat::SVGAnnotation::SVGAnnotation(const glo::ref_ptr<glat::AbstractState>& initialState, std::string svgPathString, unsigned width, unsigned height, const glat::RendererFactory& factory /*= RendererFactory()*/)
	: SVGAnnotation(initialState, factory) {
	setPathString(svgPathString);
	setWidth(width);
	setHeight(height);
}

void glat::SVGAnnotation::setPathString(std::string svgPathString) {
	setDirty(true);
	m_svgPathString = svgPathString;
}

const std::string& glat::SVGAnnotation::getPathString() const {
	return m_svgPathString;
}

unsigned glat::SVGAnnotation::getHeight() const {
	return m_height;
}

unsigned glat::SVGAnnotation::getWidth() const {
	return m_width;
}

void glat::SVGAnnotation::setWidth(unsigned width) {
	setDirty(true);
	m_width = width;
}

void glat::SVGAnnotation::setHeight(unsigned height) {
	setDirty(true);
	m_height = height;
}
