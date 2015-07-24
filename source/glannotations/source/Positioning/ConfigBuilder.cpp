#include <glannotations/Positioning/ConfigBuilder.h>
#include "glannotations-version.h"
#include <glannotations/Styles/Styles.h>

#include <rapidjson/document.h>
#include <rapidjson/reader.h>

#include <glm/glm.hpp>
#include <iostream>
#include <fstream>
#include <sstream>

using namespace rapidjson;

std::string getFileContents(std::string configPath) {
	std::string configFile = RESOURCES_DIR;
	configFile += configPath;
	std::ifstream in(configFile);
	if (in.fail() || !in.good()) {
		// no config found - return empty json
		return std::string("{ }");
	}
	std::stringstream buffer;
	buffer << in.rdbuf();
	return buffer.str();
}

template<typename T>
void processStandardElements(T& ele, const rapidjson::Value& object) {
	if (object.HasMember("type")) ele.type = object["type"].GetString();
	if (object.HasMember("content")) ele.contentDataKey = object["content"].GetString();
	if (object.HasMember("priority")) ele.priority = object["priority"].GetUint();
	if (object.HasMember("lineHeight")) ele.lineHeight = static_cast<float>(object["lineHeight"].GetDouble());

	if (object.HasMember("style")) {
		// stylings
		processStyles(ele.stylings, object["style"]);
	}
}

void processStyles(std::vector < globjects::ref_ptr<glannotations::Styling> >& stylings, const rapidjson::Value& styleObject)  {
	for (Value::ConstMemberIterator styleItr = styleObject.MemberBegin(); styleItr != styleObject.MemberEnd(); ++styleItr) {
		if (strcmp(styleItr->name.GetString(), "outline") == 0) {
			glm::vec3 color(styleItr->value["color"][0u].GetDouble(), styleItr->value["color"][1u].GetDouble(), styleItr->value["color"][2u].GetDouble());
			stylings.push_back(new glannotations::Styles::Outline(static_cast<float>(styleItr->value["width"].GetDouble()), color));
		} else {
			throw std::runtime_error("Invalid Styling in Config");
		}
	}
}

std::string genericValueToString(const rapidjson::Value& val) {
	if (val.IsString()) {
		return val.GetString();
	}
	if (val.IsDouble()) {
		return std::to_string(val.GetDouble());
	}
	if (val.IsInt()) {
		return std::to_string(val.GetInt());
	}
	throw std::runtime_error("Invalid Param type");
}

glannotations::ConfigBuilder::ConfigBuilder(std::string configPath) {
	Document document;
	document.Parse<0>(getFileContents(configPath).c_str());

	for (Value::ConstMemberIterator itr = document.MemberBegin(); itr != document.MemberEnd(); ++itr) {
		auto config = new AnnotationClassConfig;
		processStandardElements(*config, itr->value);

		for (SizeType i = 0; i < itr->value["techniques"].Size(); ++i) {
			AnnotationTechniqueConfig technique;

			technique.technique = itr->value["techniques"][i]["technique"].GetString();
			processStandardElements(technique, itr->value["techniques"][i]);

			// optional additional technique parameter
			if (itr->value["techniques"][i].HasMember("criteria"))
				for (Value::ConstMemberIterator criteriaItr = itr->value["techniques"][i]["criteria"].MemberBegin(); criteriaItr != itr->value["techniques"][i]["criteria"].MemberEnd(); ++criteriaItr)
					technique.critera.push_back(std::make_pair(criteriaItr->name.GetString(), static_cast<float>(criteriaItr->value.GetDouble())));


			// criteria
			if (itr->value["techniques"][i].HasMember("params"))
				for (Value::ConstMemberIterator criteriaItr = itr->value["techniques"][i]["params"].MemberBegin(); criteriaItr != itr->value["techniques"][i]["params"].MemberEnd(); ++criteriaItr)
					technique.additionalParams.push_back(std::make_pair(criteriaItr->name.GetString(), genericValueToString(criteriaItr->value)));

			config->techniques.push_back(technique);
		}

		m_classConfigs[itr->name.GetString()] = config;
	}
}

globjects::ref_ptr<glannotations::AnnotationClassConfig> glannotations::ConfigBuilder::getAnnotationClassConfig(std::string name) const {
	return m_classConfigs.at(name);
}

bool glannotations::ConfigBuilder::hasAnnotationClassConfig(std::string name) const {
	return m_classConfigs.count(name) == 1;
}

const std::unordered_map<std::string, globjects::ref_ptr<glannotations::AnnotationClassConfig> >& glannotations::ConfigBuilder::getAnnotationClassConfigs() const {
	return m_classConfigs;
}
