#include <glannotations/Positioning/ObjectAlignedBoundingBox.h>
#include <algorithm>
#include <vector>

glannotations::ObjectAlignedBoundingBox::ObjectAlignedBoundingBox(glm::vec3 llf, glm::vec3 llb, glm::vec3 lrf, glm::vec3 ulf) {
	m_llf = llf;
	m_llb = llb;
	m_lrf = lrf;
	m_ulf = ulf;

	//or any other kind of notice that this object is not a real Box
	//if(!isValid())
	//	throw std::runtime_error("This Box is not a valid Box!");
}

glannotations::ObjectAlignedBoundingBox::ObjectAlignedBoundingBox(glm::vec3 positionLln, glm::vec3 width, glm::vec3 length, float height) {
	m_llf = positionLln;
	m_llb = m_llf + length;
	m_lrf = m_llf + width;
	
	glm::vec3 up = glm::cross(width, length);
	m_ulf = m_llf + up;
	//todo: how can me make sure that the up-vector has the right direction?
	//glm::cross(length, width) results in -up.
	//and we don't know which width-vec and length-vec the user will give us!
	//may be check the up-result using camera's up-vec?

	if (!isValid())
		throw std::runtime_error("This Box is not a valid Box!");
}

double glannotations::ObjectAlignedBoundingBox::getVolume() const  {
	double x = (m_lrf - m_llf).length();
	double y = (m_ulf - m_llf).length();
	double z = (m_llb - m_llf).length();

	return x*y*z;
}

glm::vec3 glannotations::ObjectAlignedBoundingBox::urf() const {
	return m_ulf + (m_lrf - m_llf);
}

glm::vec3 glannotations::ObjectAlignedBoundingBox::ulb() const {
	return m_ulf + (m_llb - m_llf);
}

glm::vec3 glannotations::ObjectAlignedBoundingBox::lrb() const {
	return m_lrf + (m_llb - m_llf);
}

glm::vec3 glannotations::ObjectAlignedBoundingBox::urb() const {
	return urf() + (m_llb - m_llf);
}

double glannotations::ObjectAlignedBoundingBox::distanceToCamera(const gloperate::Camera& cam) const  {
	//find nearest vertex to cam
	
	std::vector<glm::vec3> vertices;
 
	vertices.push_back(m_llf - cam.eye());
	vertices.push_back(m_ulf - cam.eye());
	vertices.push_back(m_lrf - cam.eye());
	vertices.push_back(urf() - cam.eye());

	vertices.push_back(m_llb - cam.eye());
	vertices.push_back(ulb() - cam.eye());
	vertices.push_back(lrb() - cam.eye());
	vertices.push_back(urb() - cam.eye());

	double minDistance = cam.zFar();

	for (int i = 0; i < 8; i++) {
		minDistance = std::min((double)vertices[i].length(), minDistance);
	}

	return minDistance;
}

glm::vec3 glannotations::ObjectAlignedBoundingBox::getCentroid() const  {
	return (m_llf + urb()) / 2.f;
}

bool glannotations::ObjectAlignedBoundingBox::isValid() const  {
	
	//all vertices are different / no edge is zero
	if (m_llf == m_llb || m_llf == m_lrf || m_llf == m_ulf
		|| m_llb == m_lrf || m_llb == m_ulf
		|| m_lrf == m_ulf
		)
		return false;

	//Box: orthogonal edges
	double a = glm::dot(m_llb - m_llf, m_lrf - m_llf);
	double b = glm::dot(m_llb - m_llf, m_ulf - m_llf);
	double c = glm::dot(m_lrf - m_llf, m_ulf - m_llf);
	
	double t = std::numeric_limits<double>::epsilon();
	
	if ((a < t && a > -t)
		|| (b < t && b > -t)
		|| (c < t && c > -t))
		return false;

	return true;
}

std::array<glm::vec3, 4> glannotations::ObjectAlignedBoundingBox::getBoundingBox() {
	return std::array<glm::vec3, 4> {{ m_llf, m_lrf, m_ulf, m_llb }};
}
