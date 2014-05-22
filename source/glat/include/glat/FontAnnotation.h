#pragma once

#include <glm/glm.hpp>

#include <glat/AbstractAnnotation.h>
#include <glat/glat_api.h>

namespace glat {
	class GLAT_API FontAnnotation : public AbstractAnnotation {
	public:
		FontAnnotation(glat::AbstractState* initialState, const glat::RendererFactory& factory = RendererFactory());
		FontAnnotation(glat::AbstractState* initialState, std::string text, const glat::RendererFactory& factory = RendererFactory());
		FontAnnotation(glat::AbstractState* initialState, std::string text, glm::vec4 color, const glat::RendererFactory& factory = RendererFactory());
		void setText(std::string text);
		const std::string& getText();
		void setFontName(std::string fontName);
		const std::string& getFontName();
		void setColor(glm::vec4 color);
		const glm::vec4 getColor();
		
		bool isTextDirty();
		void setTextDirty(bool);

		inline virtual void setDirty(bool dirty);

	protected:
		std::string m_text;
		std::string m_fontName;
		glm::vec4 m_color;
		bool m_textDirty = true;
	};
}