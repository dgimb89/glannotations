#include <glannotations/Utility/BoundingBox.h>
#include <osg/BoundingBox>

#include <glannotations-wrapper/OSGAnnotationDrawable.h>

glannotations_wrapper::DrawableAnnotation::DrawableAnnotation() {
}

glannotations_wrapper::DrawableAnnotation::DrawableAnnotation(const DrawableAnnotation& drawable, const osg::CopyOp& copyop /*= osg::CopyOp::SHALLOW_COPY*/) : osg::Drawable(drawable, copyop) {
	// ToDo : Make deep copy possible
	setAnnotation(drawable.getAnnotation());
}

void glannotations_wrapper::DrawableAnnotation::drawImplementation(osg::RenderInfo&) const {
	if (m_annotation) {
		m_annotation->draw();
	}
}

osg::BoundingBox glannotations_wrapper::DrawableAnnotation::computeBoundingBox() const {
	glannotations::BoundingBox bb = m_annotation->getRenderState()->getBoundingBox();
	return osg::BoundingBox(bb.getLLF().x, bb.getLLF().y, bb.getLLF().z, bb.getURB().x, bb.getURB().y, bb.getURB().z);
}

void glannotations_wrapper::DrawableAnnotation::setAnnotation(globjects::ref_ptr<glannotations::AbstractAnnotation> annotation) {
	m_annotation = annotation;
}

const globjects::ref_ptr<glannotations::AbstractAnnotation>& glannotations_wrapper::DrawableAnnotation::getAnnotation() const {
	return m_annotation;
}
