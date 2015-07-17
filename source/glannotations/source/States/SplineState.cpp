#include <glannotations/States/SplineState.h>
#include <glannotations/Renderer/AbstractRenderer.h>
#include <glannotations/AbstractAnnotation.h>
#include <glannotations/States/StateInterpolation.h>

#include <glm/gtx/rotate_vector.hpp>

#include <iostream>

glannotations::SplineState::SplineState(glm::vec3 position
	, std::vector<glm::vec3> splineBaseControlPoints, std::vector<float> splineBaseKnotValues
	, glm::vec3 upToTopSpline)
{
	initialize(position, false);

	m_splineBase = std::shared_ptr<BSpline>(new BSpline3D(splineBaseControlPoints, splineBaseKnotValues));
	m_splineTop = std::shared_ptr<BSpline>(new BSpline3D(splineBaseControlPoints, splineBaseKnotValues));

	calculateSplineTop(upToTopSpline);
}

glannotations::SplineState::SplineState(glm::vec3 position
	, std::vector<glm::vec3> splineBaseControlPoints, unsigned int baseDegree
	, glm::vec3 upToTopSpline)
{
	initialize(position, false);

	m_splineBase = std::shared_ptr<BSpline>(new BSpline3D(splineBaseControlPoints, baseDegree));
	m_splineTop = std::shared_ptr<BSpline>(new BSpline3D(splineBaseControlPoints, baseDegree));

	calculateSplineTop(upToTopSpline);
}

glannotations::SplineState::SplineState(glm::vec3 position
	, std::vector<glm::vec3> splineBaseControlPoints, std::vector<float> splineBaseKnotValues
	, std::vector<glm::vec3> splineTopControlPoints, std::vector<float> splineTopKnotValues)
{
	initialize(position, false);

	m_splineBase = std::shared_ptr<BSpline>(new BSpline3D(splineBaseControlPoints, splineBaseKnotValues));
	m_splineTop = std::shared_ptr<BSpline>(new BSpline3D(splineTopControlPoints, splineTopKnotValues));
}

glannotations::SplineState::SplineState(glm::vec3 position
	, std::vector<glm::vec3> splineBaseControlPoints, unsigned int baseDegree
	, std::vector<glm::vec3> splineTopControlPoints, unsigned int topDegree)
{
	initialize(position, false);

	m_splineBase = std::shared_ptr<BSpline>(new BSpline3D(splineBaseControlPoints, baseDegree));
	m_splineTop = std::shared_ptr<BSpline>(new BSpline3D(splineTopControlPoints, topDegree));
}

glannotations::SplineState::SplineState(glm::vec3 position
	, glm::vec3 planeAxisX, glm::vec3 planeAxisY
	, std::vector<glm::vec2> splineBaseControlPoints, std::vector<float> splineBaseKnotValues
	, glm::vec2 upToTopSpline)
{
	initialize(position, true);

	m_splineBase = std::shared_ptr<BSpline>(new BSpline2D(planeAxisX, planeAxisY, splineBaseControlPoints, splineBaseKnotValues));
	m_splineTop = std::shared_ptr<BSpline>(new BSpline2D(planeAxisX, planeAxisY, splineBaseControlPoints, splineBaseKnotValues));
	
	calculateSplineTop(upToTopSpline);
}

glannotations::SplineState::SplineState(glm::vec3 position
	, glm::vec3 planeAxisX, glm::vec3 planeAxisY
	, std::vector<glm::vec2> splineBaseControlPoints, unsigned int baseDegree
	, glm::vec2 upToTopSpline)
{
	initialize(position, true);

	m_splineBase = std::shared_ptr<BSpline>(new BSpline2D(planeAxisX, planeAxisY, splineBaseControlPoints, baseDegree));
	m_splineTop = std::shared_ptr<BSpline>(new BSpline2D(planeAxisX, planeAxisY, splineBaseControlPoints, baseDegree));

	calculateSplineTop(upToTopSpline);
}

glannotations::SplineState::SplineState(glm::vec3 position
	, glm::vec3 planeAxisX, glm::vec3 planeAxisY
	, std::vector<glm::vec2> splineBaseControlPoints, std::vector<float> splineBaseKnotValues
	, std::vector<glm::vec2> splineTopControlPoints, std::vector<float> splineTopKnotValues)
{
	initialize(position, true);

	m_splineBase = std::shared_ptr<BSpline>(new BSpline2D(planeAxisX, planeAxisY, splineBaseControlPoints, splineBaseKnotValues));
	m_splineTop = std::shared_ptr<BSpline>(new BSpline2D(planeAxisX, planeAxisY, splineTopControlPoints, splineTopKnotValues));
}

glannotations::SplineState::SplineState(glm::vec3 position
	, glm::vec3 planeAxisX, glm::vec3 planeAxisY
	, std::vector<glm::vec2> splineBaseControlPoints, unsigned int baseDegree
	, std::vector<glm::vec2> splineTopControlPoints, unsigned int topDegree)
{
	initialize(position, true);

	m_splineBase = std::shared_ptr<BSpline>(new BSpline2D(planeAxisX, planeAxisY, splineBaseControlPoints, baseDegree));
	m_splineTop = std::shared_ptr<BSpline>(new BSpline2D(planeAxisX, planeAxisY, splineTopControlPoints, topDegree));
}

void glannotations::SplineState::initialize(glm::vec3 position, bool is2DSpline) {
	m_valid = true; //may be set false by following functions in constructor
	m_acceptsExternalReference = is2DSpline;
	m_ll = position;
}

bool glannotations::SplineState::isValid() const {
	return m_valid;
}

void glannotations::SplineState::changeOrientation(glm::vec2 newUp) {
	calculateSplineTop(newUp);
}

void glannotations::SplineState::changeOrientation(glm::vec3 newUp) {
	calculateSplineTop(newUp);
}

void glannotations::SplineState::changeOrientation(std::shared_ptr<BSpline3D> splineTop) {
	if (m_acceptsExternalReference) {
		std::cerr << "#Warning: Aborting changeOrientation due to incompatibility, because got a 3D-TopSpline for 2D-BaseSpline.\n";
		return;
	}

	(m_splineTop->asBSpline3D()).setControlPoints(splineTop->getControlPoints());
	m_splineTop->setKnotValues(splineTop->getKnotValues());
	m_splineTop->calculateSplineDegree();
}

void glannotations::SplineState::changeOrientation(std::shared_ptr<BSpline2D> splineTop) {
	if (!m_acceptsExternalReference) {
		std::cerr << "#Warning: Aborting changeOrientation due to incompatibility: got a 2D-TopSpline for 3D-BaseSpline.\n";
		return;
	}

	//splineTop_plane == splineBase_plane?
	if ((m_splineTop->asBSpline2D()).isInSamePlane(splineTop)) {

		(m_splineTop->asBSpline2D()).setControlPoints(splineTop->getControlPoints2D());
		m_splineTop->setKnotValues(splineTop->getKnotValues());
		m_splineTop->calculateSplineDegree();
	}
	else
		std::cerr << "#Warning: Aborting changeOrientation due to incompatibility: new TopSpline is not in same plane as old TopSpline.\n";
}

void glannotations::SplineState::calculateSplineTop(glm::vec3 upVecInWorldSpace) {
	//for Spline3D only
	if (!m_acceptsExternalReference) {

		auto ctrlPoints = m_splineBase->getControlPoints();
		for (size_t i = 0; i < ctrlPoints.size(); i++) {
			ctrlPoints[i] += upVecInWorldSpace;
		}
		(m_splineTop->asBSpline3D()).setControlPoints(ctrlPoints);

		m_splineTop->setKnotValues(m_splineBase->getKnotValues());
	}
	else {
		//unable to calculate splineTop
		//mark SplineState as invalid
		std::cerr << "#Error: Marking Spline as invalid, could not calculate spline top, given incompatible 3D-up vector for 2D-Spline.\n";
		m_valid = false;
	}

	setDirty(true);
}

void glannotations::SplineState::calculateSplineTop(glm::vec2 upVecInPlaneSpace) {
	//for BSpline2D only
	if (m_acceptsExternalReference){
		auto ctrlPoints = (m_splineBase->asBSpline2D()).getControlPoints2D();
		for (size_t i = 0; i < ctrlPoints.size(); i++) {
			ctrlPoints[i] += upVecInPlaneSpace;
		}
		(m_splineTop->asBSpline2D()).setControlPoints(ctrlPoints);

		m_splineTop->setKnotValues(m_splineBase->getKnotValues());
	}
	else {
		//unable to calculate splineTop
		//mark SplineState as invalid
		std::cerr << "#Error: Marking Spline as invalid, could not calculate spline top, given incompatible 2D-up vector for 3D-Spline.\n";
		m_valid = false;
	}

	setDirty(true);
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

void glannotations::SplineState::updateExtends(glm::vec2 /*sourceExtends*/){
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
