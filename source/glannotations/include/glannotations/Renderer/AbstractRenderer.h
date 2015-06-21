#pragma once

#include <globjects/base/ref_ptr.h>

#include <glannotations/Common/DirtyFlagObject.h>

namespace glannotations {

	// forward declaration 
	class AbstractAnnotation;
	class ViewportState;
	class QuadState;
	class SplineState;

	class AbstractRenderer : public glannotations::DirtyFlagObject {
		friend class AbstractState;
		friend class ViewportState;
		friend class QuadState;
		friend class SplineState;
	public:
		AbstractRenderer(unsigned int globalMatricesBindingIndex);
		virtual void prepare(const globjects::ref_ptr<glannotations::AbstractAnnotation>& annotation) = 0;
		virtual void draw(const globjects::ref_ptr<glannotations::AbstractAnnotation>& annotation) = 0;
		unsigned int getMatricesBindingIndex() const;

	protected:
		unsigned int m_globalMatricesBindingIndex;
		virtual void drawSetupState(const globjects::ref_ptr<glannotations::AbstractAnnotation>& annotation, glannotations::ViewportState& state) const = 0;
		virtual void drawSetupState(const globjects::ref_ptr<glannotations::AbstractAnnotation>& annotation, glannotations::QuadState& state) const = 0;
		virtual void drawSetupState(const globjects::ref_ptr<glannotations::AbstractAnnotation>& annotation, glannotations::SplineState& state) const = 0;
	};

}