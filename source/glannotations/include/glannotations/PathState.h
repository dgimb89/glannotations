#pragma once

#include <glannotations/ReferenceableState.h>
#include <glannotations/AbstractExternalReference.h>
#include <glannotations/BSpline.h>
#include <glannotations/glannotations_api.h>

namespace glat {
	class GLANNOTATIONS_API PathState : public glat::ReferenceableState {
	public:
		virtual bool isValid();
		virtual globjects::ref_ptr<glat::AbstractState> clone();
		virtual void setExternalReference(const globjects::ref_ptr<glat::AbstractExternalReference>& reference);

	protected:
		virtual void draw(const AbstractRenderer& renderer) override;

		virtual globjects::ref_ptr<AbstractState> interpolateWith(const InternalState& mixState, float mix);
		virtual globjects::ref_ptr<AbstractState> interpolateWith(const PathState& mixState, float mix);
		virtual globjects::ref_ptr<AbstractState> interpolateWith(const ViewportState& mixState, float mix);

	private:
		std::shared_ptr<glat::BSpline> m_path;
		globjects::ref_ptr<glat::AbstractExternalReference> m_externalReference = nullptr;
	};
}