#include <glat/GlyphSetConfig.h>
#include "config.h"

#include <iostream>
#include <fstream>
#include <sstream>

#include <rapidjson/document.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/prettywriter.h>

#define GLYPH_CONFIG "glyphconfig.json"

glat::GlyphSetConfig::GlyphSetConfig(std::string fontFileName) {
	m_fontFileName = fontFileName;

	rapidjson::Document document;	
	document.Parse<0>(getFileContent().c_str());
	if (document.HasMember(fontFileName.c_str())) {
		// deserialize begins here
		const rapidjson::Value& fontObject = document[fontFileName.c_str()];

		m_startGlyph = fontObject["startGlyph"].GetUint();
		const rapidjson::Value& glyphConfigs = fontObject["glyphs"];
		for (rapidjson::SizeType i = 0; i < glyphConfigs.Size(); ++i) {
			m_glyphConfigs.push_back(	glat::GlyphSetConfig::GlyphConfig(
											glyphConfigs[i]["llf_x"].GetDouble(),
											glyphConfigs[i]["llf_y"].GetDouble(),
											glyphConfigs[i]["urb_x"].GetDouble(),
											glyphConfigs[i]["urb_y"].GetDouble()));
		}
		setDirty(false);
	}
}

void glat::GlyphSetConfig::serialize() {
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

			singleGlyph.AddMember("llf_x", glyphConfig.llf.x, allocator);
			singleGlyph.AddMember("llf_y", glyphConfig.llf.y, allocator);

			singleGlyph.AddMember("urb_x", glyphConfig.urb.x, allocator);
			singleGlyph.AddMember("urb_y", glyphConfig.urb.y, allocator);

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

std::string glat::GlyphSetConfig::getFileContent() {
	std::string configFile = RESOURCES_DIR;
	configFile += GLYPH_CONFIG;
	std::ifstream in(configFile);
	if (in.fail() || !in.good()) {
		// config non-existant - creating new one and set to dirty
		setDirty(true);
		return std::string("{ }");
	}
	std::stringstream buffer;
	buffer << in.rdbuf();
	return buffer.str();
}

unsigned short glat::GlyphSetConfig::getNumGlyphs() {
	return m_glyphConfigs.size();
}

void glat::GlyphSetConfig::setStartGlyph(unsigned short startGlyph) {
	setDirty(true);
	m_startGlyph = startGlyph;
}

unsigned short glat::GlyphSetConfig::getStartGlyph() {
	return m_startGlyph;
}

void glat::GlyphSetConfig::setNormalizedGlyphConfigs(const std::vector<GlyphConfig>& glyphConfigs) {
	setDirty(true);
	m_glyphConfigs = glyphConfigs;
}

const std::vector<glat::GlyphSetConfig::GlyphConfig>& glat::GlyphSetConfig::getGlyphConfigs() {
	return m_glyphConfigs;
}

const glat::GlyphSetConfig::GlyphConfig& glat::GlyphSetConfig::getGlyphConfig(unsigned short numGlyph) {
	return m_glyphConfigs.at(numGlyph);
}

void glat::GlyphSetConfig::setGlyphConfigs(const std::vector<GlyphConfig>& glyphConfigs, double maxWidth, double maxHeight) {
	setDirty(true);
	setNormalizedGlyphConfigs(glyphConfigs);
	for (auto& glyphConfig : m_glyphConfigs) {
		glyphConfig.llf.x /= maxWidth;
		glyphConfig.llf.y /= maxHeight;
		glyphConfig.urb.x /= maxWidth;
		glyphConfig.urb.y /= maxHeight;
	}
}

const glat::GlyphSetConfig::GlyphConfig& glat::GlyphSetConfig::getGlyphConfigForCharcode(unsigned short charCode) {
	return getGlyphConfig(charCode - m_startGlyph);
}

glat::GlyphSetConfig::GlyphConfig::GlyphConfig(glm::highp_float llf_x, glm::highp_float llf_y, glm::highp_float urb_x, glm::highp_float urb_y) {
	this->llf = glm::highp_vec2(llf_x, llf_y);
	this->urb = glm::highp_vec2(urb_x, urb_y);
}
