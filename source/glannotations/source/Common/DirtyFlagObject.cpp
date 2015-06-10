#include <glannotations/Common/DirtyFlagObject.h>

bool glannotations::DirtyFlagObject::isDirty() const {
	return m_dirty;
}

void glannotations::DirtyFlagObject::setDirty(bool dirty) const {
	m_dirty = dirty;
}

glannotations::DirtyFlagObject::DirtyFlagObject()
	: m_dirty(true) {
}

glannotations::DirtyFlagObject::~DirtyFlagObject() {

}

