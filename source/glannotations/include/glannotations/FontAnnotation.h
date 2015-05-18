#pragma once

#include <glm/glm.hpp>

#include <glannotations/AbstractAnnotation.h>
#include <glannotations/glannotations_api.h>

namespace glannotations {
	class GLANNOTATIONS_API FontAnnotation : public AbstractAnnotation {
	public:
		FontAnnotation(const globjects::ref_ptr<glannotations::AbstractState>& initialState, std::string text, std::string fontName, const glannotations::RendererFactory& factory = RendererFactory());

		void setText(std::string text);
		const std::string& getText() const;
		void setFontName(std::string fontName);
		const std::string& getFontName() const;
		void setColor(glm::vec4 color);
		const glm::vec4 getColor() const;
		void setFontSize(float ptSize);
		float getFontSize() const;

		virtual void setDirty(bool dirty);

	protected:
		std::string m_text, m_fontName;
		float m_fontPtSize = 72.f;
		glm::vec4 m_color = glm::vec4(0.f, 0.f, 0.f, 1.f);
	};
}
