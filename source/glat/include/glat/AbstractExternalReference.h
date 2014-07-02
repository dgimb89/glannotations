#pragma once

#include <glbinding/types.h>
#include <glowutils/Camera.h>

#include <glat/DirtyFlagObject.h>
#include <glat/glat_api.h>

namespace glat {

	class InternalState;
	class PathState;
	class AbstractRenderer;

	class GLAT_API AbstractExternalReference : public glat::DirtyFlagObject {
	public:
		friend class ReferenceableState;
		friend class InternalState;
		friend class PathState;
		AbstractExternalReference(bool positioningOnly);
		bool isPositioningOnly();

	protected:
		inline void setPositioningOnly(bool positioningOnly);
		inline void setBindingIndex(gl::GLuint bindingIndex);
		inline gl::GLuint getBindingIndex() const;

		virtual void draw() = 0;
		virtual void updateBindings(const glat::AbstractRenderer& renderer) = 0;
		virtual void setupExternalReference(const InternalState& state);
		virtual void setupExternalReference(const PathState& state);

		// updates positioning of given state
		virtual void updatePositioning(InternalState& state) = 0;
		virtual void updatePositioning(PathState& state) = 0;

		bool m_positioningOnly;
		gl::GLuint m_bindingIndex;

	private:
		void assertNotReused();
	};
}