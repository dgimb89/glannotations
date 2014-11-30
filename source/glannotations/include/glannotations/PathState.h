#pragma once

#include <glannotations/ReferenceableState.h>
#include <glannotations/AbstractExternalReference.h>
#include <glannotations/BSpline.h>
#include <glannotations/glannotations_api.h>

namespace glannotations {
	class GLANNOTATIONS_API PathState : public glannotations::ReferenceableState {
	public:
		virtual bool isValid();
		virtual globjects::ref_ptr<glannotations::AbstractState> clone();
		virtual void setExternalReference(const globjects::ref_ptr<glannotations::AbstractExternalReference>& reference);

	protected:
		virtual void draw(const AbstractRenderer& renderer) override;

		virtual globjects::ref_ptr<AbstractState> interpolateWith(const InternalState& mixState, float mix);
		virtual globjects::ref_ptr<AbstractState> interpolateWith(const PathState& mixState, float mix);
		virtual globjects::ref_ptr<AbstractState> interpolateWith(const ViewportState& mixState, float mix);

	private:
		std::shared_ptr<glannotations::BSpline> m_path;
		globjects::ref_ptr<glannotations::AbstractExternalReference> m_externalReference = nullptr;
	};
}