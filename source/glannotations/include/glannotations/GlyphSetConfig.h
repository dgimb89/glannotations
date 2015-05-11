#pragma once

#include <glm/glm.hpp>
#include <string>
#include <vector>

#include <glannotations/glannotations_api.h>
#include <glannotations/DirtyFlagObject.h>

template class GLANNOTATIONS_API std::basic_string < char, std::char_traits<char>, std::allocator<char> > ;
template struct GLANNOTATIONS_API glm::detail::tvec2 < float, glm::precision::defaultp > ;

// disable export warnings for std::vector<GlyphConfig> because we dont offer it in interface
#  pragma warning( push )
#  pragma warning( disable: 4251 )

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
		size_t getNumGlyphs();
		void setStartGlyph(size_t startGlyph);
		void setWhitespaceLength(float whitespaceLength);
		float getWhitespaceLength() const;
		size_t getStartGlyph();

		/// normalizes given configs by dividing every glyph coordinates by maxWidth resp. maxHeight
		void setGlyphConfigs(const std::vector<glannotations::GlyphSetConfig::GlyphConfig>& glyphConfigs, size_t maxWidth, size_t maxHeight);
		const std::vector<glannotations::GlyphSetConfig::GlyphConfig>& getGlyphConfigs();
		const glannotations::GlyphSetConfig::GlyphConfig& getGlyphConfig(size_t numGlyph);
		const glannotations::GlyphSetConfig::GlyphConfig& getGlyphConfigForCharcode(size_t charCode);

	protected: 
		std::string getFileContent();
		size_t m_startGlyph = 0;
		float m_whitespaceLength;
		std::string m_fontFileName;

	private:
		void setNormalizedGlyphConfigs(const std::vector<glannotations::GlyphSetConfig::GlyphConfig>& glyphConfigs);
		std::vector<glannotations::GlyphSetConfig::GlyphConfig> m_glyphConfigs;
	};
}

#  pragma warning( pop )