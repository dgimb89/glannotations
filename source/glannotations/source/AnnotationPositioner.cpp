#include <glannotations/AnnotationPositioner.h>
#include <glannotations/Positioning/ConfigBuilder.h>

glannotations::AnnotationPositioner::AnnotationPositioner(const globjects::ref_ptr<AnnotationSpace> &annotationSpace) {
	m_annotationSpace = annotationSpace;
}

std::vector<globjects::ref_ptr<glannotations::AnnotationGroup> > glannotations::AnnotationPositioner::generateAnnotationGroups(std::string configPath) const {
	ConfigBuilder configs(configPath);

	std::vector<globjects::ref_ptr<glannotations::AnnotationGroup> > result;
	for (const auto & config : configs.getAnnotationClassConfigs()) {
		auto description = new AnnotationDescription(config.first, config.second);
		auto group = new AnnotationGroup();
		// todo: use config to fill annotation group
		group->setAnnotationPositioner(shared_from_this(), description);
		result.push_back(group);
	}
	return result;
}

void glannotations::AnnotationPositioner::updateAnnotation(const globjects::ref_ptr<const AnnotationDescription>& description, const globjects::ref_ptr<glannotations::AbstractAnnotation>& annotation) const {
	// todo evaluate dynamic metrics
}