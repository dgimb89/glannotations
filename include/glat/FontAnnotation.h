#ifndef GLAT_FONT_ANNOTATION_H
#define GLAT_FONT_ANNOTATION_H

#include <glat/AbstractAnnotation.h>

namespace glat {
	class FontAnnotation : public AbstractAnnotation {
	public:
		FontAnnotation(glat::AbstractState* initialState, const glat::RendererFactory& factory = RendererFactory());
		FontAnnotation(glat::AbstractState* initialState, std::string text, const glat::RendererFactory& factory = RendererFactory());
		void setText(std::string text);
		const std::string& getText();
		void setFontName(std::string fontName);
		const std::string& getFontName();

	protected:
		std::string m_text;
		std::string m_fontName;
	};
}

#endif