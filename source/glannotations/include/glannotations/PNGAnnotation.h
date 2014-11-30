#pragma once

#include <glm/glm.hpp>

#include <glannotations/AbstractAnnotation.h>
#include <glannotations/glannotations_api.h>

namespace glannotations {
	class GLANNOTATIONS_API PNGAnnotation : public AbstractAnnotation
	{
	public:
		PNGAnnotation(const globjects::ref_ptr<glannotations::AbstractState>& initialState, std::string fileName, const glannotations::RendererFactory& factory = RendererFactory());
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