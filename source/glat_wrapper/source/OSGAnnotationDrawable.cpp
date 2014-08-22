#include <glat/BoundingBox.h>
#include <osg/BoundingBox>

#include <glat/wrapper/OSGAnnotationDrawable.h>

glat::wrapper::DrawableAnnotation::DrawableAnnotation() {
}

glat::wrapper::DrawableAnnotation::DrawableAnnotation(const DrawableAnnotation& drawable, const osg::CopyOp& copyop /*= osg::CopyOp::SHALLOW_COPY*/) : osg::Drawable(drawable, copyop) {
	// ToDo : Make deep copy possible
	setAnnotation(drawable.getAnnotation());
}

void glat::wrapper::DrawableAnnotation::drawImplementation(osg::RenderInfo&) const {
	if (m_annotation) {
		m_annotation->draw();
	}
}

osg::BoundingBox glat::wrapper::DrawableAnnotation::computeBound() const {
	glat::BoundingBox bb = m_annotation->getRenderState()->getBoundingBox();
	return osg::BoundingBox(-10.f, -10.f, -10.f, 10.f, 10.f, 10.f);
}

void glat::wrapper::DrawableAnnotation::setAnnotation(glow::ref_ptr<glat::AbstractAnnotation> annotation) {
	m_annotation = annotation;
}

const glow::ref_ptr<glat::AbstractAnnotation>& glat::wrapper::DrawableAnnotation::getAnnotation() const {
	return m_annotation;
}
