#ifndef GLAT_DISTANCE_FIELD_MAPPING_H
#define GLAT_DISTANCE_FIELD_MAPPING_H

#include <glat/AbstractRenderer.h>

namespace glat {
	class DistanceFieldRenderer : public AbstractRenderer {
	public:
		virtual void draw(glat::AbstractAnnotation* annotation);
	};
}

#endif