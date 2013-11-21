#ifndef DISTANCE_FIELD_MAPPING_H
#define DISTANCE_FIELD_MAPPING_H

#include <glat/AbstractRenderMethod.h>

namespace glat {
	class DistanceFieldMapping : public AbstractRenderMethod {
	public:
		virtual void render(glat::AbstractAnnotation* annotation);
	};
}

#endif