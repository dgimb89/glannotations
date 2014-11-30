#include <limits>

#include <glannotations/BoundingBox.h>

glannotations::BoundingBox::BoundingBox() {
	m_llf = glm::vec3(std::numeric_limits<float>::max());
	m_urb = glm::vec3(std::numeric_limits<float>::lowest());
}

glannotations::BoundingBox::BoundingBox(glm::vec3 llf, glm::vec3 urb) {
	m_llf = llf;
	m_urb = urb;
}

void glannotations::BoundingBox::extendBy(glm::vec3 vertex) {
	m_urb.x = std::fmaxf(m_urb.x, vertex.x);
	m_urb.y = std::fmaxf(m_urb.y, vertex.y);
	m_urb.z = std::fmaxf(m_urb.z, vertex.z);

	m_llf.x = std::fminf(m_llf.x, vertex.x);
	m_llf.y = std::fminf(m_llf.y, vertex.y);
	m_llf.z = std::fminf(m_llf.z, vertex.z);
}

const glm::vec3& glannotations::BoundingBox::getLLF() const {
	return m_llf;
}

const glm::vec3& glannotations::BoundingBox::getURB() const {
	return m_urb;
}

