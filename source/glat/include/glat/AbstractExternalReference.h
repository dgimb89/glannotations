#pragma once

#include <glowutils/Camera.h>

#include <glat/DirtyFlagObject.h>
#include <glat/glat_api.h>

namespace glat {

	class InternalState;
	class InternalPathState;

	class GLAT_API AbstractExternalReference : public glat::DirtyFlagObject {
	public:
		AbstractExternalReference(glowutils::Camera* camera, bool positioningOnly);

		virtual void draw() = 0;
		inline void setCamera(glowutils::Camera* camera);
		inline void setPositioningOnly(bool positioningOnly);
		bool isPositioningOnly();

		// updates positioning of given state
		virtual void updatePositioning(InternalState& state) = 0;
		virtual void updatePositioning(InternalPathState& state) = 0;

	protected:
		glowutils::Camera* m_camera;
		bool m_positioningOnly;
	};
}