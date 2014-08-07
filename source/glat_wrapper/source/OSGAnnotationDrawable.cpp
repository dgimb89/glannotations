#include <glat/BoundingBox.h>
#include <osg/BoundingBox>

#include <glat/wrapper/OSGAnnotationDrawable.h>

glat::wrapper::DrawableAnnotation::DrawableAnnotation() {

}

glat::wrapper::DrawableAnnotation::DrawableAnnotation(const DrawableAnnotation& teapot, const osg::CopyOp& copyop /*= osg::CopyOp::SHALLOW_COPY*/) : osg::Drawable(teapot, copyop) {
	// ToDo : Make deep copy possible
	setAnnotation(teapot.getAnnotation());
}

void glat::wrapper::DrawableAnnotation::drawImplementation(osg::RenderInfo&) const {
	if (m_annotation) {
		m_annotation->draw();
	}
}

osg::BoundingBox glat::wrapper::DrawableAnnotation::computeBoundingBox() const {
	glat::BoundingBox bb = m_annotation->getRenderState()->getBoundingBox();
	return osg::BoundingBox(bb.getLLF().x, bb.getLLF().y, bb.getLLF().z, bb.getURB().x, bb.getURB().y, bb.getURB().z);
}

void glat::wrapper::DrawableAnnotation::setAnnotation(glow::ref_ptr<glat::AbstractAnnotation> annotation) {
	m_annotation = annotation;
}

const glow::ref_ptr<glat::AbstractAnnotation>& glat::wrapper::DrawableAnnotation::getAnnotation() const {
	return m_annotation;
}
