#include <glat/Object.h>

bool glat::Object::isDirty() const {
	return m_dirty;
}

void glat::Object::setDirty(bool dirty) const {
	m_dirty = dirty;
}

glat::Object::Object()
	: m_dirty(true) {
}

