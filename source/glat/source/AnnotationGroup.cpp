#include <glat/AnnotationGroup.h>

void glat::AnnotationGroup::addAnnotation(const glo::ref_ptr<glat::AbstractAnnotation>& annotation) {
	// Todo push back if viewport state
	m_annotations.push_front(annotation);
}

void glat::AnnotationGroup::draw() {
	for (auto& annotation : m_annotations) {
		annotation->draw();
	}
}