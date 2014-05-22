#pragma once

#include <glowbase/Referenced.h>

#include <glat/glat_api.h>

namespace glat {
	class GLAT_API DirtyFlagObject : public glow::Referenced {
	public:
		DirtyFlagObject();
		virtual bool isDirty() const;
		inline virtual void setDirty(bool) const;

	protected: 
		mutable bool m_dirty;
	};
}
