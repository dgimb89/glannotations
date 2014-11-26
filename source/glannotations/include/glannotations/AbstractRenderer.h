#pragma once

#include <globjects/base/ref_ptr.h>

#include <glannotations/DirtyFlagObject.h>

namespace glat {

	// forward declaration 
	class AbstractAnnotation;
	class AbstractExternalState; class ViewportState;
	class InternalState;
	class PathState;

	class AbstractRenderer : public glat::DirtyFlagObject {
		friend class ViewportState;
		friend class InternalState;
		friend class PathState;
	public:
		AbstractRenderer(unsigned int globalMatricesBindingIndex);
		virtual void draw(const globjects::ref_ptr<glat::AbstractAnnotation>& annotation) = 0;
		unsigned int getMatricesBindingIndex() const;

	protected:
		unsigned int m_globalMatricesBindingIndex;
		virtual void drawSetupState(const glat::ViewportState& state) const = 0;
		virtual void drawSetupState(const glat::InternalState& state) const = 0;
		virtual void drawSetupState(const glat::PathState& state) const = 0;
	};

}