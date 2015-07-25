#include <glannotations/AnnotationPositioner.h>
#include <glannotations/Positioning/ConfigBuilder.h>

#include <glannotations/FontAnnotation.h>
#include <glannotations/PNGAnnotation.h>

#include <glannotations/States/ViewportState.h>
#include <glannotations/States/QuadState.h>

#include <glannotations/Externals/BoxReference.h>
#include <glannotations/Externals/FlagReference.h>
#include <glannotations/Externals/LabelReference.h>

glannotations::AnnotationPositioner::AnnotationPositioner(const globjects::ref_ptr<AnnotationSpace> &annotationSpace) {
	m_annotationSpace = annotationSpace;
}

std::vector<globjects::ref_ptr<glannotations::AnnotationGroup> > glannotations::AnnotationPositioner::generateAnnotationGroups(std::string configPath) const {
	ConfigBuilder configs(configPath);

	std::vector<globjects::ref_ptr<glannotations::AnnotationGroup> > result;
	for (const auto & config : configs.getAnnotationClassConfigs()) {
		if (!m_annotationSpace->classHasSpaceObjects(config.first)) {
			continue;
		}
		globjects::ref_ptr<glannotations::AnnotationDescription> description = new AnnotationDescription(config.first, config.second);
		globjects::ref_ptr<glannotations::AnnotationGroup> group = new AnnotationGroup();
		auto objects = m_annotationSpace->getSpaceObjectsForClass(config.first);
		for (const auto& object : objects) {
			group->addAnnotation(createAnnotationFor(object, description, 0));
		}
		group->setAnnotationPositioner(shared_from_this(), description);
		result.push_back(group);
	}
	return result;
}

globjects::ref_ptr<glannotations::AbstractAnnotation> glannotations::AnnotationPositioner::createAnnotationFor(const globjects::ref_ptr<glannotations::SpaceObject>& object, globjects::ref_ptr<AnnotationDescription> description, unsigned techniqueIndex) const {
	auto config = description->getAnnotationClassConfig();

	globjects::ref_ptr<glannotations::AbstractState> state;
	{
		auto technique = config->getAttributeForTechnique("technique", techniqueIndex);
		auto extends = object->getBoundingBox();
		if (technique == "top") {
			state = new glannotations::QuadState(extends.at(2), extends.at(2) + (extends.at(1) - extends.at(0)), extends.at(2) + (extends.at(1) - extends.at(0)) + (extends.at(3) - extends.at(0)));
		} else if (technique == "label") {
			auto centroid = object->getCentroid() + (extends.at(2) - extends.at(0));
			state = new glannotations::QuadState(centroid - glannotations::getRight(), centroid + glannotations::getRight(), centroid + glannotations::getUp() + glannotations::getRight());
			state->asQuadState().setExternalReference(new glannotations::LabelReference(centroid));
		} else if (technique == "flag") {
			auto up = extends.at(2) - extends.at(0);
			state = new glannotations::QuadState(
				extends.at(0) + glm::normalize(up) * std::stof(config->getAttributeForTechnique("flagHeight", techniqueIndex)), 
				extends.at(1) + glm::normalize(up) * std::stof(config->getAttributeForTechnique("flagHeight", techniqueIndex)), 
				extends.at(1) + up);
			state->asQuadState().setExternalReference(new glannotations::FlagReference(std::stof(config->getAttributeForTechnique("flagWidth", techniqueIndex)), extends.at(3) - extends.at(0)));
		} else if (technique == "box"){
			state = new glannotations::QuadState(extends.at(0), extends.at(1), extends.at(2) + (extends.at(1) - extends.at(0)));
				state->asQuadState().setExternalReference(new glannotations::BoxReference(glm::vec2(), glm::vec2(), extends.at(3) - extends.at(0),
					!(config->hasAttributeForTechnique("showReference", techniqueIndex) && config->getAttributeForTechnique("showReference", techniqueIndex) != "false")));
		} else {
			throw glannotations::InvalidConfigError((std::string("Invalid annotation technique: ") + technique).c_str());
		}

		if(config->hasAttributeForTechnique("lineHeight", techniqueIndex)) {
			state->setMaximumHeight(std::stof(config->getAttributeForTechnique("lineHeight", techniqueIndex)));
		}
		state->setKeepSourceAspectRatio(	!config->hasAttributeForTechnique("ignoreAspectRatio", techniqueIndex) 
										||  !(config->getAttributeForTechnique("ignoreAspectRatio", techniqueIndex) == "true"));
		for (const auto& style : config->stylings){
			state->setStyling(style);
		}
		for (const auto& style : config->techniques.at(techniqueIndex).stylings) {
			state->setStyling(style);
		}
	}

	globjects::ref_ptr<glannotations::AbstractAnnotation> annotation;
	{
		auto type = config->getAttributeForTechnique("type", techniqueIndex);
		auto content = config->getAttributeForTechnique("content", techniqueIndex);
		if (type == "font") {
			annotation = new glannotations::FontAnnotation(state, object->getData(content), config->getAttributeForTechnique("font", techniqueIndex));
		} else if (type == "png") {
			annotation = new glannotations::PNGAnnotation(state, object->getData(content));
		} else {
			throw glannotations::InvalidConfigError((std::string("Invalid annotation type: ") + type).c_str());
		}
	}

	// set annotation space reference for later updates
	annotation->setRespectiveSpaceObject(object);
	annotation->setCurrentFallback(techniqueIndex);

	return annotation;
}


void glannotations::AnnotationPositioner::updateAnnotation(const globjects::ref_ptr<AnnotationDescription>& description, const globjects::ref_ptr<glannotations::AbstractAnnotation>& annotation) const {
	// todo evaluate dynamic metrics
	// -> via transform feedback with previous object id texture
}