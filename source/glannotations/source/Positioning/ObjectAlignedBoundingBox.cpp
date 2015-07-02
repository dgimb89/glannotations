#include <glannotations/Positioning/ObjectAlignedBoundingBox.h>

#include <vector>

glannotations::ObjectAlignedBoundingBox::ObjectAlignedBoundingBox(glm::vec3 lln, glm::vec3 llf, glm::vec3 lrn, glm::vec3 uln) {
	m_lln = lln;
	m_llf = llf;
	m_lrn = lrn;
	m_uln = uln;

	//or any other kind of notice that this object is not a real Box
	if(!isValid())
		throw std::runtime_error("This Box is not a valid Box!");
}

glannotations::ObjectAlignedBoundingBox::ObjectAlignedBoundingBox(glm::vec3 positionLln, glm::vec3 width, glm::vec3 length, float height) {
	m_lln = positionLln;
	m_llf = m_lln + length;
	m_lrn = m_lln + width;
	
	glm::vec3 up = glm::cross(width, length);
	m_uln = m_lln + up;
	//todo: how can me make sure that the up-vector has the right direction?
	//glm::cross(length, width) results in -up.
	//and we don't know which width-vec and length-vec the user will give us!
	//may be check the up-result using camera's up-vec?

	if (!isValid())
		throw std::runtime_error("This Box is not a valid Box!");
}

double glannotations::ObjectAlignedBoundingBox::getVolume() const  {
	double x = (m_lrn - m_lln).length();
	double y = (m_uln - m_lln).length();
	double z = (m_llf - m_lln).length();

	return x*y*z;
}

glm::vec3 glannotations::ObjectAlignedBoundingBox::urn() const {
	return m_uln + (m_lrn - m_lln);
}

glm::vec3 glannotations::ObjectAlignedBoundingBox::ulf() const {
	return m_uln + (m_llf - m_lln);
}

glm::vec3 glannotations::ObjectAlignedBoundingBox::lrf() const {
	return m_lrn + (m_llf - m_lln);
}

glm::vec3 glannotations::ObjectAlignedBoundingBox::urf() const {
	return urn() + (m_llf - m_lln);
}

double glannotations::ObjectAlignedBoundingBox::distanceToCamera(const gloperate::Camera& cam) const  {
	//find nearest vertex to cam
	
	std::vector<glm::vec3> vertices;
 
	vertices.push_back(m_lln - cam.eye());
	vertices.push_back(m_uln - cam.eye());
	vertices.push_back(m_lrn - cam.eye());
	vertices.push_back(urn() - cam.eye());

	vertices.push_back(m_llf - cam.eye());
	vertices.push_back(ulf() - cam.eye());
	vertices.push_back(lrf() - cam.eye());
	vertices.push_back(urf() - cam.eye());

	double minDistance = cam.zFar();

	for (int i = 0; i < 8; i++) {
		minDistance = min((double)vertices[i].length(), minDistance);
	}

	return minDistance;
}

glm::vec3 glannotations::ObjectAlignedBoundingBox::getCentroid() const  {
	
	glm::vec3 diagonal = lrf() - m_uln;

	return diagonal * (diagonal.length() / 2.f);
}

bool glannotations::ObjectAlignedBoundingBox::isValid() const  {
	
	//all vertices are different / no edge is zero
	if (m_lln == m_llf || m_lln == m_lrn || m_lln == m_uln
		|| m_llf == m_lrn || m_llf == m_uln
		|| m_lrn == m_uln
		)
		return false;

	//Box: orthogonal edges
	double a = glm::dot(m_llf - m_lln, m_lrn - m_lln);
	double b = glm::dot(m_llf - m_lln, m_uln - m_lln);
	double c = glm::dot(m_lrn - m_lln, m_uln - m_lln);
	
	double t = std::numeric_limits<double>::epsilon();
	
	if ((a < t && a > -t)
		|| (b < t && b > -t)
		|| (c < t && c > -t))
		return false;

	return true;
}