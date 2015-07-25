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
	if (val.IsBool()) {
		return val.GetBool() ? "true" : "false";
	}
	throw glannotations::InvalidConfigError("Invalid Param type");
}

template<typename T>
void processStandardElements(T& ele, const rapidjson::Value& object) {
	for (Value::ConstMemberIterator itr = object.MemberBegin(); itr != object.MemberEnd(); ++itr) {
		std::string key(itr->name.GetString());
		if (key == "style" || key == "techniques" || key == "criteria") {
			continue;
		}
		ele.attr[key] = genericValueToString(itr->value);
	}

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
			throw glannotations::InvalidConfigError("Invalid Styling in Config");
		}
	}
}

glannotations::ConfigBuilder::ConfigBuilder(std::string configPath) {
	Document document;
	document.Parse<0>(getFileContents(configPath).c_str());

	for (Value::ConstMemberIterator itr = document.MemberBegin(); itr != document.MemberEnd(); ++itr) {
		auto config = new AnnotationClassConfig;
		processStandardElements(*config, itr->value);

		for (SizeType i = 0; i < itr->value["techniques"].Size(); ++i) {
			AnnotationTechniqueConfig technique;
			processStandardElements(technique, itr->value["techniques"][i]);

			// optional additional technique parameter
			if (itr->value["techniques"][i].HasMember("criteria"))
				for (Value::ConstMemberIterator criteriaItr = itr->value["techniques"][i]["criteria"].MemberBegin(); criteriaItr != itr->value["techniques"][i]["criteria"].MemberEnd(); ++criteriaItr)
					technique.critera.push_back(std::make_pair(criteriaItr->name.GetString(), static_cast<float>(criteriaItr->value.GetDouble())));

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

std::string glannotations::AnnotationClassConfig::getAttributeForTechnique(std::string attrKey, unsigned techniqueIndex) const {
	if (techniques.at(techniqueIndex).hasAttribute(attrKey)) {
		return techniques.at(techniqueIndex).getAttribute(attrKey);
	}
	return getAttribute(attrKey);
}

bool glannotations::AnnotationClassConfig::hasAttributeForTechnique(std::string attrKey, unsigned techniqueIndex) const {
	if (techniques.at(techniqueIndex).hasAttribute(attrKey)) {
		return true;
	}
	return hasAttribute(attrKey);
}

std::string glannotations::CommonConfigData::getAttribute(std::string attrKey) const {
	if (hasAttribute(attrKey)) {
		return attr.at(attrKey);
	}
	throw glannotations::InvalidConfigError((std::string("Requested data attribute missing: ") + attrKey).c_str());
}

bool glannotations::CommonConfigData::hasAttribute(std::string attrKey) const {
	return attr.count(attrKey) == 1;
}

glannotations::InvalidConfigError::InvalidConfigError(const char* m) : std::runtime_error(m) { }
