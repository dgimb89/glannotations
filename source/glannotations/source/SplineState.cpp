#include <glannotations/SplineState.h>
#include <glannotations/AbstractRenderer.h>
#include <glannotations/AbstractAnnotation.h>
#include <glannotations/StateInterpolation.h>

#include <glm/gtx/rotate_vector.hpp>

#include <iostream> //debug todo:anne remove

glannotations::SplineState::SplineState(glm::vec3 position, BSpline splineBase, GlyphOrientationOnSpline splineOrientation){
	m_ll = position;
	m_splineBase = std::shared_ptr<BSpline>(new BSpline(splineBase.getControlPoints(), splineBase.getKnotValues()));
	m_splineOrientation = splineOrientation;
	updateSplineTop();
}

glannotations::SplineState::SplineState(glm::vec3 position, BSpline splineBase, BSpline splineTop){
	m_ll = position;
	m_splineBase = std::shared_ptr<BSpline>(new BSpline(splineBase.getControlPoints(), splineBase.getKnotValues()));
	m_splineTop = std::shared_ptr<BSpline>(new BSpline(splineTop.getControlPoints(), splineTop.getKnotValues()));
	m_splineOrientation = GlyphOrientationOnSpline::CUSTOM_SECOND_SPLINE;
}

glannotations::SplineState::SplineState(glm::vec3 position, BSpline splineBase, BSpline splineTop, GlyphOrientationOnSpline splineOrientation){
	m_ll = position;
	m_splineBase = std::shared_ptr<BSpline>(new BSpline(splineBase.getControlPoints(), splineBase.getKnotValues()));
	m_splineTop = std::shared_ptr<BSpline>(new BSpline(splineTop.getControlPoints(), splineTop.getKnotValues()));
	m_splineOrientation = splineOrientation;
}

glannotations::SplineState::SplineState(glm::vec3 position, std::vector<glm::vec3> splineBaseControlPoints, std::vector<float> splineBaseKnotValues
	, std::vector<glm::vec3> splineTopControlPoints, std::vector<float> splineTopKnotValues
	, GlyphOrientationOnSpline splineOrientation)
{
	m_ll = position;
	m_splineBase = std::shared_ptr<BSpline>(new BSpline(splineBaseControlPoints, splineBaseKnotValues));
	m_splineTop = std::shared_ptr<BSpline>(new BSpline(splineTopControlPoints, splineTopKnotValues));
	m_splineOrientation = splineOrientation;
}

bool glannotations::SplineState::isValid() const {
	//throw std::logic_error("The method or operation is not implemented.");
	//todo:anne
	return true;
}

void glannotations::SplineState::changeOrientation(GlyphOrientationOnSpline splineOrientation) {
	if (m_splineOrientation != splineOrientation){
		m_splineOrientation = splineOrientation;
		updateSplineTop();
	}
}

void glannotations::SplineState::changeOrientation(std::shared_ptr<BSpline> splineTop) {
	m_splineOrientation = GlyphOrientationOnSpline::CUSTOM_SECOND_SPLINE;

	m_splineTop->setControlPoints(splineTop->getControlPoints());
	m_splineTop->setKnotValues(splineTop->getKnotValues());
	m_splineTop->calculateSplineDegree();
}

void glannotations::SplineState::updateSplineTop() {
	setDirty(true);

	int baseCtrlPointSize = m_splineBase->getControlPoints().size() - 1;
	
	//"curve plane": described by the vector(last_controlpoint, first_controlpoint) and secantVec
	glm::vec3 v1 = (m_splineBase->getControlPoints().back() - m_splineBase->getControlPoints().front());
	glm::bvec3 result = glm::equal(v1, glm::vec3(0, 0, 0));
	if (result.x && result.y && result.z) {
		//todo:anne debug testing
		//if firstControlpoint = lastControlpoint (because Circle), then try first and second controlpoint
		//it would be a very strange curve, if first = third, too!
		//we can't use first and second, because that will be our first secantVec!
		//and I really hope that there are more than 3 controlpoints, because else first/third could be first/last!
		v1 = (m_splineBase->getControlPoints().at(2) - m_splineBase->getControlPoints().front());
	}

	glm::vec3 normal, secantVec;

	std::vector<glm::vec3> newTopCtrlPoints;

	for (int i = 0; i < baseCtrlPointSize; i++){
		secantVec = m_splineBase->getControlPoints().at(i + 1) - m_splineBase->getControlPoints().at(i);

		normal = glm::cross(v1, secantVec);

		glm::bvec3 result = glm::equal(normal, glm::vec3(0, 0, 0));
		if (result.x && result.y && result.z) {
			std::cout << "debug: warning: secant vector equals vector(first_controlpoint, last_controlpoint) of curve!.";
		}

		normal = glm::normalize(normal);

		switch (m_splineOrientation){
		case ORTHOGONAL_TO_PLANE:
			//glyphs orthogonal to the curve plane
			newTopCtrlPoints.push_back(m_splineBase->getControlPoints().at(i) + normal);
			break;
		case IN_SAME_PLANE:
			//glyphs in the same plane as the curve
			//use normal to rotate the secant vec
			newTopCtrlPoints.push_back(m_splineBase->getControlPoints().at(i) + glm::rotate(secantVec, glm::radians(90.0f), normal));
			break;
		default:
			//CUSTOM_SECOND_SPLINE
			//m_splineTop already exists, so use that.
			std::cout << "debug: warning: m_splineTop unchanged.";
			break;
		}
	}

	//last point, repeat last secantVec or normal respectively
	switch (m_splineOrientation){
	case ORTHOGONAL_TO_PLANE:
		//glyphs orthogonal to the curve plane
		newTopCtrlPoints.push_back(m_splineBase->getControlPoints().back() + normal);
		m_splineTop->setControlPoints(newTopCtrlPoints);
		m_splineTop->setKnotValues(m_splineBase->getKnotValues());
		m_splineTop->calculateSplineDegree();
		break;
	case IN_SAME_PLANE:
		//glyphs in the same plane as the curve
		//use normal to rotate the secant vec
		newTopCtrlPoints.push_back(m_splineBase->getControlPoints().back() + glm::rotate(secantVec, glm::radians(90.0f), normal));
		m_splineTop->setControlPoints(newTopCtrlPoints);
		m_splineTop->setKnotValues(m_splineBase->getKnotValues());
		m_splineTop->calculateSplineDegree();
		break;
	}
}

void glannotations::SplineState::draw(const AbstractRenderer& renderer) {
	if (m_externalReference) {
		m_externalReference->updateBindings(renderer); //?
		m_externalReference->updatePositioning(*this);
		m_externalReference->draw();
	}
	//todo:anne maybe do something here?
	renderer.drawSetupState(*this);
}

const glm::vec3& glannotations::SplineState::getLL() const {
	return m_ll;
}

globjects::ref_ptr<glannotations::AbstractState> glannotations::SplineState::interpolateWith(const InternalState& mixState, float mix) {
	return glannotations::Interpolation::interpolate(*this, mixState, mix);
}

globjects::ref_ptr<glannotations::AbstractState> glannotations::SplineState::interpolateWith(const SplineState& mixState, float mix) {
	return glannotations::Interpolation::interpolate(*this, mixState, mix);
}

globjects::ref_ptr<glannotations::AbstractState> glannotations::SplineState::interpolateWith(const ViewportState& mixState, float mix) {
	return glannotations::Interpolation::interpolate(*this, mixState, mix);
}

globjects::ref_ptr<glannotations::AbstractState> glannotations::SplineState::clone() const{
	//fucking clone shared pointer operator= private zeugs
	//"Language!", says Captain America.
	//Sorry.
	
	globjects::ref_ptr<glannotations::AbstractState> clonedState(new SplineState(
		m_ll
		, m_splineBase->getControlPoints(), m_splineBase->getKnotValues()
		, m_splineTop->getControlPoints(), m_splineTop->getKnotValues()
		, m_splineOrientation));
	ReferenceableState::copyState(dynamic_cast<ReferenceableState&> (*clonedState));
	return clonedState;
}

void glannotations::SplineState::setExternalReference(const globjects::ref_ptr<glannotations::AbstractExternalReference>& reference) {
	ReferenceableState::setExternalReference(reference);
	m_externalReference->setupExternalReference(*this);
}

void glannotations::SplineState::updateExtends(glm::vec2 sourceExtends){
	throw std::logic_error("The method or operation is not implemented.");
	//todo:anne
	//don't know how to do that, we don't have extends in this form
}

glannotations::BoundingBox glannotations::SplineState::getBoundingBox() {
	//todo maybe there is a more efficient way
	glannotations::BoundingBox bb;

	for (auto ctrlPoint : m_splineBase->getControlPoints())
	{
		bb.extendBy(ctrlPoint);
	}

	for (auto ctrlPoint : m_splineTop->getControlPoints())
	{
		bb.extendBy(ctrlPoint);
	}
	return bb;
}


glm::vec3 glannotations::SplineState::retrieveConnectingVectorAt(float t) {
	//useful for positioning
	//it might not be that orthogonal...

	glm::vec3 position_ll = m_splineBase->retrieveCurvepointAt(t);
	glm::vec3 position_ul = m_splineTop->retrieveCurvepointAt(t);

	return (position_ul - position_ll);
}

glm::vec3 glannotations::SplineState::retrieveSecantVectorAt(float t, float nextT) {
	return m_splineBase->retrieveSecantVectorAt(t, nextT);
};
