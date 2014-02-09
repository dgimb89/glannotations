#ifndef GLAT_GLYPH_SET_CONFIG_H
#define GLAT_GLYPH_SET_CONFIG_H

#include <glat/DirtyFlagObject.h>
#include <glm/glm.hpp>
#include <string>
#include <vector>

namespace glat {

	class GlyphSetConfig : public glat::DirtyFlagObject {
	public:
		struct GlyphConfig {
			glm::vec2 ll, ur;
			GlyphConfig(glm::float_t ll_x, glm::float_t ll_y, glm::float_t ur_x, glm::float_t ur_y);
		};

		GlyphSetConfig(std::string fontFileName);
		void serialize();

		std::string getGlyphsetImageName() const;
		unsigned short getNumGlyphs();
		void setStartGlyph(unsigned short startGlyph);
		unsigned short getStartGlyph();
		void setNormalizedGlyphConfigs(const std::vector<glat::GlyphSetConfig::GlyphConfig>& glyphConfigs);

		/// normalizes given configs by dividing every glyph coordinates by maxWidth resp. maxHeight
		void setGlyphConfigs(const std::vector<glat::GlyphSetConfig::GlyphConfig>& glyphConfigs, double maxWidth, double maxHeight);
		const std::vector<glat::GlyphSetConfig::GlyphConfig>& getGlyphConfigs();
		const glat::GlyphSetConfig::GlyphConfig& getGlyphConfig(unsigned short numGlyph);
		const glat::GlyphSetConfig::GlyphConfig& getGlyphConfigForCharcode(unsigned short charCode);

	protected: 
		std::string getFileContent();
		unsigned short m_startGlyph = 0;
		std::vector<glat::GlyphSetConfig::GlyphConfig> m_glyphConfigs;
		std::string m_fontFileName;
	};
}

#endif // GLAT_GLYPH_SET_CONFIG_H
