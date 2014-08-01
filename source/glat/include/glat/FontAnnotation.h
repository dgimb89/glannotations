#pragma once

#include <glm/glm.hpp>

#include <glat/AbstractAnnotation.h>
#include <glat/glat_api.h>

namespace glat {
	class GLAT_API FontAnnotation : public AbstractAnnotation {
	public:
		FontAnnotation(const glow::ref_ptr<glat::AbstractState>& initialState, std::string text, std::string fontName, const glat::RendererFactory& factory = RendererFactory());

		void setText(std::string text);
		const std::string& getText() const;
		void setFontName(std::string fontName);
		const std::string& getFontName() const;
		void setColor(glm::vec4 color);
		const glm::vec4 getColor() const;
		void setFontSize(float ptSize);
		float getFontSize() const;

		inline virtual void setDirty(bool dirty);

	protected:
		std::string m_text, m_fontName;
		float m_fontPtSize = 72.f;
		glm::vec4 m_color = glm::vec4(0.f, 0.f, 0.f, 1.f);
	};
}