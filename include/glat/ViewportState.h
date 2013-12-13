#ifndef GLAT_VIEWPORT_STATE_H
#define GLAT_VIEWPORT_STATE_H

#include <glat/AbstractState.h>

namespace glat {

	typedef struct SpatialExtends2D {
		float left, top;
	} SpatialExtends2D;

	class ViewportState : public glat::AbstractState {
	public:
		ViewportState(glat::SpatialExtends2D extends);
	};
}

#endif // GLAT_VIEWPORT_STATE_H
