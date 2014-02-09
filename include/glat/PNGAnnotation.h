#ifndef GLAT_PNG_ANNOTATION_H
#define GLAT_PNG_ANNOTATION_H

#include <glat/AbstractAnnotation.h>
#include <glm/glm.hpp>

namespace glat {
	class PNGAnnotation : public AbstractAnnotation
	{
	public:
		PNGAnnotation(glat::AbstractState* initialState, const glat::RendererFactory& factory = RendererFactory());
		PNGAnnotation(glat::AbstractState* initialState, std::string fileName, const glat::RendererFactory& factory = RendererFactory());
		void setPNGFile(std::string filename, bool isDistanceField = false);
		void setAsDistanceField(glm::vec4 renderColor);
		void setAsNoDistanceField();

	protected:
		bool m_distanceField = false;
		glm::vec4 m_color;
		std::string m_fileName;
	};
}

#endif // !GLAT_PNG_ANNOTATION_H