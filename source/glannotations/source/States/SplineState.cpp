#include <glannotations/States/SplineState.h>
#include <glannotations/Renderer/AbstractRenderer.h>
#include <glannotations/AbstractAnnotation.h>
#include <glannotations/States/StateInterpolation.h>

#include <glm/gtx/rotate_vector.hpp>

glannotations::SplineState::SplineState(glm::vec3 position
	, std::vector<glm::vec3> splineBaseControlPoints, std::vector<float> splineBaseKnotValues
	, glm::vec3 up)
{
	m_ll = position;
	m_splineBase = std::shared_ptr<BSpline>(new BSpline(splineBaseControlPoints, splineBaseKnotValues));
	m_splineTop = std::shared_ptr<BSpline>(new BSpline(splineBaseControlPoints, splineBaseKnotValues));

	calculateSplineTop(up);
}

glannotations::SplineState::SplineState(glm::vec3 position
	, std::vector<glm::vec3> splineBaseControlPoints, unsigned int baseDegree
	, glm::vec3 up)
{
	m_ll = position;
	m_splineBase = std::shared_ptr<BSpline>(new BSpline(splineBaseControlPoints, baseDegree));
	m_splineTop = std::shared_ptr<BSpline>(new BSpline(splineBaseControlPoints, baseDegree));

	calculateSplineTop(up);
}

glannotations::SplineState::SplineState(glm::vec3 position
	, std::vector<glm::vec3> splineBaseControlPoints, std::vector<float> splineBaseKnotValues
	, std::vector<glm::vec3> splineTopControlPoints, std::vector<float> splineTopKnotValues)
{
	m_ll = position;
	m_splineBase = std::shared_ptr<BSpline>(new BSpline(splineBaseControlPoints, splineBaseKnotValues));
	m_splineTop = std::shared_ptr<BSpline>(new BSpline(splineTopControlPoints, splineTopKnotValues));
}

glannotations::SplineState::SplineState(glm::vec3 position
	, std::vector<glm::vec3> splineBaseControlPoints, unsigned int baseDegree
	, std::vector<glm::vec3> splineTopControlPoints, unsigned int topDegree)
{
	m_ll = position;
	m_splineBase = std::shared_ptr<BSpline>(new BSpline(splineBaseControlPoints, baseDegree));
	m_splineTop = std::shared_ptr<BSpline>(new BSpline(splineTopControlPoints, topDegree));
}

bool glannotations::SplineState::isValid() const {
	//throw std::logic_error("The method or operation is not implemented.");
	//todo:anne
	return true;
}

void glannotations::SplineState::changeOrientation(glm::vec3 newUp) {
	calculateSplineTop(newUp);
}

void glannotations::SplineState::changeOrientation(std::shared_ptr<BSpline> splineTop) {
	m_splineTop->setControlPoints(splineTop->getControlPoints());
	m_splineTop->setKnotValues(splineTop->getKnotValues());
	m_splineTop->calculateSplineDegree();
}

void glannotations::SplineState::calculateSplineTop(glm::vec3 upVec) {
	setDirty(true);
	
	auto ctrlPoints = m_splineBase->getControlPoints();
	for (int i = 0; i < ctrlPoints.size(); i++) {
		ctrlPoints[i] += upVec;
	}
	m_splineTop->setControlPoints(ctrlPoints);

	//copy knot values because I think that this is the right thing to do... but I really have no idea!
	m_splineTop->setKnotValues(m_splineBase->getKnotValues());

}

void glannotations::SplineState::draw(const globjects::ref_ptr<glannotations::AbstractAnnotation>& annotation, const AbstractRenderer& renderer) {
	drawExternalReference(renderer, *this);
	renderer.drawSetupState(annotation, *this);
}

const glm::vec3& glannotations::SplineState::getLL() const {
	return m_ll;
}

globjects::ref_ptr<glannotations::AbstractState> glannotations::SplineState::interpolateWith(const QuadState& mixState, float mix) {
	return glannotations::Interpolation::interpolate(*this, mixState, mix);
}

globjects::ref_ptr<glannotations::AbstractState> glannotations::SplineState::interpolateWith(const SplineState& mixState, float mix) {
	return glannotations::Interpolation::interpolate(*this, mixState, mix);
}

globjects::ref_ptr<glannotations::AbstractState> glannotations::SplineState::interpolateWith(const ViewportState& mixState, float mix) {
	return glannotations::Interpolation::interpolate(*this, mixState, mix);
}

globjects::ref_ptr<glannotations::AbstractState> glannotations::SplineState::clone() const{
	globjects::ref_ptr<glannotations::AbstractState> clonedState(
		new SplineState( m_ll
		, m_splineBase->getControlPoints(), m_splineBase->getKnotValues()
		, m_splineTop->getControlPoints(), m_splineTop->getKnotValues()
		)
	);
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
	//it might be not that orthogonal...

	glm::vec3 position_ll = m_splineBase->retrieveCurvepointAt(t);
	glm::vec3 position_ul = m_splineTop->retrieveCurvepointAt(t);

	return (position_ul - position_ll);
}

glm::vec3 glannotations::SplineState::retrieveSecantVectorAt(float t, float nextT) {
	return m_splineBase->retrieveSecantVectorAt(t, nextT);
}

void glannotations::SplineState::prepare() {
	updatePositioning(*this);
	AbstractState::prepare();
}