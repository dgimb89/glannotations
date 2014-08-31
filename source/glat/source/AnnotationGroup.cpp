#include <glat/AnnotationGroup.h>

void glat::AnnotationGroup::addAnnotation(const glo::ref_ptr<glat::AbstractAnnotation>& annotation) {
	m_annotations.push_back(annotation);
}

void glat::AnnotationGroup::draw() {
	for (const auto& annotation : m_annotations) {
			annotation->draw();
	}
}