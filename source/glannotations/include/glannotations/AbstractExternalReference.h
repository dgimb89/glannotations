#pragma once

#include <globjects/base/ref_ptr.h>

#include <glannotations/DirtyFlagObject.h>
#include <glannotations/AbstractDrawingPrimitive.h>
#include <glannotations/glannotations_api.h>

namespace glat {

	class InternalState;
	class PathState;
	class AbstractRenderer;

	class GLANNOTATIONS_API AbstractExternalReference : public glat::DirtyFlagObject {
	public:
		friend class ReferenceableState;
		friend class InternalState;
		friend class PathState;
		AbstractExternalReference(bool positioningOnly);
		bool isPositioningOnly();
		void setColor(glm::vec4 color);

	protected:
		void setDirty(bool) const override;
		void setPositioningOnly(bool positioningOnly);
		void setBindingIndex(unsigned int bindingIndex);
		unsigned int getBindingIndex() const;

		virtual void draw();
		virtual void updateBindings(const glat::AbstractRenderer& renderer);
		virtual void setupExternalReference(const InternalState& state);
		virtual void setupExternalReference(const PathState& state);

		// updates positioning of given state
		virtual void updatePositioning(InternalState& state) = 0;
		virtual void updatePositioning(PathState& state) = 0;

		bool m_positioningOnly;
		unsigned int m_bindingIndex;
		globjects::ref_ptr<glat::AbstractDrawingPrimitive> m_externalPrimitive;

	private:
		void assertNotReused();
	};
}
