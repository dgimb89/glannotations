#include <glannotations/Positioning/AnnotationPositioner.h>

glannotations::AnnotationPositioner::AnnotationPositioner(const globjects::ref_ptr<AnnotationSpace> &annotationSpace) {
	m_annotationSpace = annotationSpace;
}

std::vector<globjects::ref_ptr<glannotations::AnnotationGroup> > glannotations::AnnotationPositioner::generateAnnotationGroups(std::string configPath) const {
	// todo: create config object

	std::vector<globjects::ref_ptr<const glannotations::AnnotationDescription> > descriptions;
	// todo: generate descriptions from config

	std::vector<globjects::ref_ptr<glannotations::AnnotationGroup> > result;
	for (const auto & description : descriptions) {
		auto group = new AnnotationGroup();

		// todo: use config to create annotation group
		group->setAnnotationPositioner(shared_from_this(), description);
		result.push_back(group);
	}
	return result;
}

void glannotations::AnnotationPositioner::updateAnnotation(const globjects::ref_ptr<const AnnotationDescription>& description, const globjects::ref_ptr<glannotations::AbstractAnnotation>& annotation) const {
	// todo evaluate dynamic metrics
}

glannotations::AnnotationPositioner::~AnnotationPositioner() {
}
