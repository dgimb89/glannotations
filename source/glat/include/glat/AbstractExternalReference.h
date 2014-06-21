#pragma once

#include <glowutils/Camera.h>

#include <glat/DirtyFlagObject.h>
#include <glat/glat_api.h>

namespace glat {

	class InternalState;
	class PathState;

	class GLAT_API AbstractExternalReference : public glat::DirtyFlagObject {
	public:
		friend class InternalState;
		friend class PathState;
		AbstractExternalReference(glowutils::Camera* camera, bool positioningOnly);
		bool isPositioningOnly();

	protected:
		inline void setCamera(glowutils::Camera* camera);
		inline void setPositioningOnly(bool positioningOnly);

		virtual void draw() = 0;
		virtual void setupExternalReference(const InternalState& state);
		virtual void setupExternalReference(const PathState& state);

		// updates positioning of given state
		virtual void updatePositioning(InternalState& state) = 0;
		virtual void updatePositioning(PathState& state) = 0;

		glowutils::Camera* m_camera;
		bool m_positioningOnly;

	private:
		void assertNotReused();
	};
}