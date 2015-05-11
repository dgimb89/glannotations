#pragma once

#include <globjects/base/ref_ptr.h>

#include <glannotations/DirtyFlagObject.h>
#include <glannotations/AbstractDrawingPrimitive.h>
#include <glannotations/glannotations_api.h>

namespace glannotations {

	class InternalState;
	class PathState;
	class AbstractRenderer;

	template struct GLANNOTATIONS_API glm::detail::tvec4 < float, glm::precision::defaultp > ;
	template class GLANNOTATIONS_API globjects::ref_ptr < glannotations::AbstractDrawingPrimitive > ;

	class GLANNOTATIONS_API AbstractExternalReference : public glannotations::DirtyFlagObject {
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
		virtual void updateBindings(const glannotations::AbstractRenderer& renderer);
		virtual void setupExternalReference(const InternalState& state);
		virtual void setupExternalReference(const PathState& state);

		// updates positioning of given state
		virtual void updatePositioning(InternalState& state) = 0;
		virtual void updatePositioning(PathState& state) = 0;

		globjects::ref_ptr<glannotations::AbstractDrawingPrimitive> m_externalPrimitive;

	private:
		void assertNotReused();
			
		bool m_positioningOnly;
		unsigned int m_bindingIndex;
		glm::vec4 m_color = glm::vec4(1.f, 0.f, 0.f, .35f);
	};
}
