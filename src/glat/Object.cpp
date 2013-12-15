#include <glat/Object.h>

bool glat::Object::isDirty() {
	return m_dirty;
}

void glat::Object::setDirty(bool dirty) {
	m_dirty = dirty;
}

glat::Object::Object()
	: m_dirty(true) {
}

