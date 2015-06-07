#pragma once

#include <globjects/base/ref_ptr.h>

#include <glannotations/Common/DirtyFlagObject.h>
#include <glannotations/Renderer/AbstractDrawingPrimitive.h>
#include <glannotations/glannotations_api.h>

namespace glannotations {

	class InternalState;
	class SplineState;
	class AbstractRenderer;

	class GLANNOTATIONS_API AbstractExternalReference : public glannotations::DirtyFlagObject {

		friend class ReferenceableState;
		friend class InternalState;
		friend class SplineState;

	public:
		AbstractExternalReference(bool positioningOnly);
		bool isPositioningOnly() const;
		void setColor(glm::vec4 color);

	protected:
		void setPositioningOnly(bool positioningOnly);
		void setBindingIndex(unsigned int bindingIndex);
		bool bindingIndexSet() const;
		unsigned int getBindingIndex() const;

		virtual void draw();
		virtual void updateBindings(const glannotations::AbstractRenderer& renderer);
		virtual void setupExternalReference(const InternalState& state);
		virtual void setupExternalReference(const SplineState& state);

		// updates positioning of given state
		virtual void updatePositioning(InternalState& state) = 0;
		virtual void updatePositioning(SplineState& state) = 0;

		bool m_positioningOnly;
		unsigned int m_bindingIndex;
		bool m_bindingIndexSet = false;
		globjects::ref_ptr<glannotations::AbstractDrawingPrimitive> m_externalPrimitive;
		glm::vec4 m_color = glm::vec4(1.f, 0.f, 0.f, .35f);

	private:
		void setDirty(bool) const override;
		void assertNotReused();
	};
}
