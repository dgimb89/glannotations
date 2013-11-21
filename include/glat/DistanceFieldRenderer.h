#ifndef DISTANCE_FIELD_MAPPING_H
#define DISTANCE_FIELD_MAPPING_H

#include <glat/AbstractRenderer.h>

namespace glat {
	class DistanceFieldRenderer : public AbstractRenderer {
	public:
		virtual void render(glat::AbstractAnnotation* annotation);
	};
}

#endif