#ifndef GLAT_DIRTY_FLAG_OBJECT_H
#define GLAT_DIRTY_FLAG_OBJECT_H

#include <glow/Referenced.h>

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

#endif // GLAT_DIRTY_FLAG_OBJECT_H
