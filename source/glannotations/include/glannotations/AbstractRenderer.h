#pragma once

#include <globjects/base/ref_ptr.h>

#include <glannotations/DirtyFlagObject.h>

namespace glannotations {

	// forward declaration 
	class AbstractAnnotation;
	class AbstractExternalState; class ViewportState;
	class InternalState;
	class PathState;

	class AbstractRenderer : public glannotations::DirtyFlagObject {
		friend class ViewportState;
		friend class InternalState;
		friend class PathState;
	public:
		AbstractRenderer(unsigned int globalMatricesBindingIndex);
		virtual void prepare(const globjects::ref_ptr<glannotations::AbstractAnnotation>& annotation) = 0;
		virtual void draw(const globjects::ref_ptr<glannotations::AbstractAnnotation>& annotation) = 0;
		unsigned int getMatricesBindingIndex() const;

	protected:
		unsigned int m_globalMatricesBindingIndex;
		virtual void drawSetupState(const glannotations::ViewportState& state) const = 0;
		virtual void drawSetupState(const glannotations::InternalState& state) const = 0;
		virtual void drawSetupState(const glannotations::PathState& state) const = 0;
	};

}