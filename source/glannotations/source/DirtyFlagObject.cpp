#include <glannotations/DirtyFlagObject.h>

bool glat::DirtyFlagObject::isDirty() const {
	return m_dirty;
}

void glat::DirtyFlagObject::setDirty(bool dirty) const {
	m_dirty = dirty;
}

glat::DirtyFlagObject::DirtyFlagObject()
	: m_dirty(true) {
}

