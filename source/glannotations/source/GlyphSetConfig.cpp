#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>

#include <rapidjson/document.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/prettywriter.h>

#include <glannotations/GlyphSetConfig.h>
#include "glannotations-version.h"

#define GLYPH_CONFIG "glyphconfig.json"

glannotations::GlyphSetConfig::GlyphSetConfig(std::string fontFileName) {
	if (fontFileName == "")
		throw std::runtime_error("Font file can not be empty!");

	m_fontFileName = fontFileName;

	rapidjson::Document document;	
	document.Parse<0>(getFileContent().c_str());
	if (document.HasMember(fontFileName.c_str())) {
		// deserialization begins here
		const rapidjson::Value& fontObject = document[fontFileName.c_str()];

		m_startGlyph = fontObject["startGlyph"].GetUint();
		const rapidjson::Value& glyphConfigs = fontObject["glyphs"];
		for (rapidjson::SizeType i = 0; i < glyphConfigs.Size(); ++i) {
			m_glyphConfigs.push_back(	glannotations::GlyphSetConfig::GlyphConfig(
											glyphConfigs[i]["llf_x"].GetDouble(),
											glyphConfigs[i]["llf_y"].GetDouble(),
											glyphConfigs[i]["advance_x"].GetDouble(),
											glyphConfigs[i]["advance_y"].GetDouble()));
		}
		setDirty(false);
	}
}

void glannotations::GlyphSetConfig::serialize() {
	if (!isDirty()) return;
	rapidjson::Document document;
	document.Parse<0>(getFileContent().c_str());
	rapidjson::Document::AllocatorType& allocator = document.GetAllocator();

	// we have all these scopes so that rapidjson can do memcpy optimization

	rapidjson::Value fontObject(rapidjson::kObjectType);
	fontObject.AddMember("startGlyph", m_startGlyph, allocator);

	{
		rapidjson::Value glyphs(rapidjson::kArrayType);
		for (auto glyphConfig : m_glyphConfigs) {
			rapidjson::Value singleGlyph(rapidjson::kObjectType);

			singleGlyph.AddMember("llf_x", glyphConfig._ll.x, allocator);
			singleGlyph.AddMember("llf_y", glyphConfig._ll.y, allocator);

			singleGlyph.AddMember("advance_x", glyphConfig._advance.x, allocator);
			singleGlyph.AddMember("advance_y", glyphConfig._advance.y, allocator);

			glyphs.PushBack(singleGlyph, allocator);
		}
		fontObject.AddMember("glyphs", glyphs, allocator);
	}

	if (document.HasMember(m_fontFileName.c_str()))
		document.RemoveMember(m_fontFileName.c_str());

	document.AddMember(m_fontFileName.c_str(), fontObject, allocator);

	// new json document generated - now output to file
	rapidjson::StringBuffer os;
	rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(os);
	document.Accept(writer);

	std::string configFile = RESOURCES_DIR;
	configFile += GLYPH_CONFIG;
	std::ofstream out(configFile);
	if (out.fail()) {
		std::cerr << "failed to write json config" << std::endl;
		return;
	}
	out << os.GetString();
	out.close();
	setDirty(false);
}

std::string glannotations::GlyphSetConfig::getFileContent() {
	std::string configFile = RESOURCES_DIR;
	configFile += GLYPH_CONFIG;
	std::ifstream in(configFile);
	if (in.fail() || !in.good()) {
		// config non-existent - creating new one and set to dirty
		setDirty(true);
		return std::string("{ }");
	}
	std::stringstream buffer;
	buffer << in.rdbuf();
	return buffer.str();
}

unsigned short glannotations::GlyphSetConfig::getNumGlyphs() {
	return m_glyphConfigs.size();
}

void glannotations::GlyphSetConfig::setStartGlyph(unsigned short startGlyph) {
	setDirty(true);
	m_startGlyph = startGlyph;
}

unsigned short glannotations::GlyphSetConfig::getStartGlyph() {
	return m_startGlyph;
}

void glannotations::GlyphSetConfig::setNormalizedGlyphConfigs(const std::vector<GlyphConfig>& glyphConfigs) {
	setDirty(true);
	m_glyphConfigs = glyphConfigs;
}

const std::vector<glannotations::GlyphSetConfig::GlyphConfig>& glannotations::GlyphSetConfig::getGlyphConfigs() {
	return m_glyphConfigs;
}

const glannotations::GlyphSetConfig::GlyphConfig& glannotations::GlyphSetConfig::getGlyphConfig(unsigned short numGlyph) {
	return m_glyphConfigs.at(numGlyph);
}

void glannotations::GlyphSetConfig::setGlyphConfigs(const std::vector<GlyphConfig>& glyphConfigs, double maxWidth, double maxHeight) {
	setDirty(true);
	setNormalizedGlyphConfigs(glyphConfigs);
	for (auto& glyphConfig : m_glyphConfigs) {
		glyphConfig._ll.x /= maxWidth;
		glyphConfig._ll.y /= maxHeight;
		glyphConfig._advance.x /= maxWidth;
		glyphConfig._advance.y /= maxHeight;
	}
}

const glannotations::GlyphSetConfig::GlyphConfig& glannotations::GlyphSetConfig::getGlyphConfigForCharcode(unsigned short charCode) {
	return getGlyphConfig(charCode - m_startGlyph);
}

std::string glannotations::GlyphSetConfig::getGlyphsetImageName() const {
	return std::string(m_fontFileName + ".png");
}

glannotations::GlyphSetConfig::GlyphConfig::GlyphConfig(glm::float_t ll_x, glm::float_t ll_y, glm::float_t advance_x, glm::float_t advance_y) {
	_ll = glm::highp_vec2(ll_x, ll_y);
	_advance = glm::highp_vec2(advance_x, advance_y);
}
