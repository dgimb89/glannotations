#ifndef GLAT_OBJECT_H
#define GLAT_OBJECT_H

#include <glow/Referenced.h>

namespace glat {
	class Object : public glow::Referenced {
	public:
		Object();
		virtual bool isDirty() const;
		inline virtual void setDirty(bool) const;

	protected: 
		mutable bool m_dirty;
	};
}

#endif // GLAT_OBJECT_H
