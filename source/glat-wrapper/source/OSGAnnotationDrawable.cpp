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
		// try hard ... but doesn't work ... todo
		glPushAttrib(GL_ALL_ATTRIB_BITS);
		m_annotation->draw();
		glPopAttrib();
	}
}

osg::BoundingBox glat::wrapper::DrawableAnnotation::computeBound() const {
	glat::BoundingBox bb = m_annotation->getRenderState()->getBoundingBox();
	return osg::BoundingBox(bb.getLLF().x, bb.getLLF().y, bb.getLLF().z, bb.getURB().x, bb.getURB().y, bb.getURB().z);
}

void glat::wrapper::DrawableAnnotation::setAnnotation(glo::ref_ptr<glat::AbstractAnnotation> annotation) {
	m_annotation = annotation;
}

const glo::ref_ptr<glat::AbstractAnnotation>& glat::wrapper::DrawableAnnotation::getAnnotation() const {
	return m_annotation;
}
