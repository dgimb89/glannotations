#pragma once

#include <glowbase/Referenced.h>

namespace glat {
	class DirtyFlagObject : public glow::Referenced {
	public:
		DirtyFlagObject();
		virtual bool isDirty() const;
		inline virtual void setDirty(bool) const;

	protected: 
		mutable bool m_dirty;
	};
}
