#pragma once

#include <globjects/base/Referenced.h>

#include <glannotations/glannotations_api.h>

namespace glannotations {
	class GLANNOTATIONS_API DirtyFlagObject : public globjects::Referenced {
	public:
		DirtyFlagObject();
		virtual bool isDirty() const;
		virtual void setDirty(bool) const;

	protected: 
		mutable bool m_dirty;
	};
}
