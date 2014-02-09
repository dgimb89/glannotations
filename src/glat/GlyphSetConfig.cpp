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
	if (fontFileName == "")
		throw std::runtime_error("Font file can not be empty!");

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

			singleGlyph.AddMember("llf_x", glyphConfig.ll.x, allocator);
			singleGlyph.AddMember("llf_y", glyphConfig.ll.y, allocator);

			singleGlyph.AddMember("urb_x", glyphConfig.ur.x, allocator);
			singleGlyph.AddMember("urb_y", glyphConfig.ur.y, allocator);

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
		// config non-existent - creating new one and set to dirty
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
		glyphConfig.ll.x /= maxWidth;
		glyphConfig.ll.y /= maxHeight;
		glyphConfig.ur.x /= maxWidth;
		glyphConfig.ur.y /= maxHeight;
	}
}

const glat::GlyphSetConfig::GlyphConfig& glat::GlyphSetConfig::getGlyphConfigForCharcode(unsigned short charCode) {
	return getGlyphConfig(charCode - m_startGlyph);
}

std::string glat::GlyphSetConfig::getGlyphsetImageName() const {
	return std::string(m_fontFileName + ".png");
}

glat::GlyphSetConfig::GlyphConfig::GlyphConfig(glm::float_t ll_x, glm::float_t ll_y, glm::float_t ur_x, glm::float_t ur_y) {
	this->ll = glm::highp_vec2(ll_x, ll_y);
	this->ur = glm::highp_vec2(ur_x, ur_y);
}
