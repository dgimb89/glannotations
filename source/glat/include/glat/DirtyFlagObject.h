#pragma once

#include <globjects-base/Referenced.h>

#include <glat/glat_api.h>

namespace glat {
	class GLAT_API DirtyFlagObject : public glo::Referenced {
	public:
		DirtyFlagObject();
		virtual bool isDirty() const;
		virtual void setDirty(bool) const;

	protected: 
		mutable bool m_dirty;
	};
}
