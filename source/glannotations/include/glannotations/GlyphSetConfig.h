#pragma once

#include <glm/glm.hpp>
#include <string>
#include <vector>

#include <glannotations/glannotations_api.h>
#include <glannotations/DirtyFlagObject.h>

namespace glannotations {

	class GLANNOTATIONS_API GlyphSetConfig : public glannotations::DirtyFlagObject {
	public:
		struct GLANNOTATIONS_API GlyphConfig {
			glm::vec2 _ll, _advance;
			GlyphConfig(glm::float_t ll_x, glm::float_t ll_y, glm::float_t advance_x, glm::float_t advance_y);
		};

		GlyphSetConfig(std::string fontFileName);
		void serialize();

		std::string getGlyphsetImageName() const;
		unsigned short getNumGlyphs();
		void setStartGlyph(unsigned short startGlyph);
		void setWhitespaceLength(float whitespaceLength);
		float getWhitespaceLength() const;
		unsigned short getStartGlyph();
		void setNormalizedGlyphConfigs(const std::vector<glannotations::GlyphSetConfig::GlyphConfig>& glyphConfigs);

		/// normalizes given configs by dividing every glyph coordinates by maxWidth resp. maxHeight
		void setGlyphConfigs(const std::vector<glannotations::GlyphSetConfig::GlyphConfig>& glyphConfigs, double maxWidth, double maxHeight);
		const std::vector<glannotations::GlyphSetConfig::GlyphConfig>& getGlyphConfigs();
		const glannotations::GlyphSetConfig::GlyphConfig& getGlyphConfig(unsigned short numGlyph);
		const glannotations::GlyphSetConfig::GlyphConfig& getGlyphConfigForCharcode(unsigned short charCode);

	protected: 
		std::string getFileContent();
		unsigned short m_startGlyph = 0;
		float m_whitespaceLength;
		std::vector<glannotations::GlyphSetConfig::GlyphConfig> m_glyphConfigs;
		std::string m_fontFileName;
	};
}
