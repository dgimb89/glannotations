#pragma once

#include <glat/AbstractAnnotation.h>
#include <glm/glm.hpp>

namespace glat {
	class PNGAnnotation : public AbstractAnnotation
	{
	public:
		PNGAnnotation(glat::AbstractState* initialState, std::string fileName, const glat::RendererFactory& factory = RendererFactory());
		void setPNGFile(std::string filename, bool isDistanceField = false);
		void setAsDistanceField(glm::vec4 renderColor);
		void setAsNoDistanceField();

		const std::string& getFileName() const;
		bool isDistanceField() const;
		glm::vec4 getColor() const;

	protected:
		bool m_distanceField = false;
		glm::vec4 m_color;
		std::string m_fileName;
	};
}