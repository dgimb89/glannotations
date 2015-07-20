#include <glannotations/States/SplineState.h>
#include <glannotations/Renderer/AbstractRenderer.h>
#include <glannotations/AbstractAnnotation.h>
#include <glannotations/States/StateInterpolation.h>

#include <glm/gtx/rotate_vector.hpp>

#include <iostream>
#include <math.h>

glannotations::SplineState::SplineState(glm::vec3 position
	, std::vector<glm::vec3> splineBaseControlPoints, std::vector<float> splineBaseKnotValues
	, glm::vec3 upToTopSpline)
{
	initialize3D(position);

	m_splineBase = std::shared_ptr<BSpline>(new BSpline3D(splineBaseControlPoints, splineBaseKnotValues));
	m_splineTop = std::shared_ptr<BSpline>(new BSpline3D(splineBaseControlPoints, splineBaseKnotValues));

	calculateSplineTop(upToTopSpline);
}

glannotations::SplineState::SplineState(glm::vec3 position
	, std::vector<glm::vec3> splineBaseControlPoints, unsigned int baseDegree
	, glm::vec3 upToTopSpline)
{
	initialize3D(position);

	m_splineBase = std::shared_ptr<BSpline>(new BSpline3D(splineBaseControlPoints, baseDegree));
	m_splineTop = std::shared_ptr<BSpline>(new BSpline3D(splineBaseControlPoints, baseDegree));

	calculateSplineTop(upToTopSpline);
}

glannotations::SplineState::SplineState(glm::vec3 position
	, std::vector<glm::vec3> splineBaseControlPoints, std::vector<float> splineBaseKnotValues
	, std::vector<glm::vec3> splineTopControlPoints, std::vector<float> splineTopKnotValues)
{
	initialize3D(position);

	m_splineBase = std::shared_ptr<BSpline>(new BSpline3D(splineBaseControlPoints, splineBaseKnotValues));
	m_splineTop = std::shared_ptr<BSpline>(new BSpline3D(splineTopControlPoints, splineTopKnotValues));
}

glannotations::SplineState::SplineState(glm::vec3 position
	, std::vector<glm::vec3> splineBaseControlPoints, unsigned int baseDegree
	, std::vector<glm::vec3> splineTopControlPoints, unsigned int topDegree)
{
	initialize3D(position);

	m_splineBase = std::shared_ptr<BSpline>(new BSpline3D(splineBaseControlPoints, baseDegree));
	m_splineTop = std::shared_ptr<BSpline>(new BSpline3D(splineTopControlPoints, topDegree));
}

glannotations::SplineState::SplineState(glm::vec3 position
	, glm::vec3 planeAxisDirection, glm::vec3 planeAxisUp
	, std::vector<glm::vec2> splineBaseControlPoints, std::vector<float> splineBaseKnotValues
	, glm::vec2 upToTopSpline)
{
	initialize2D(position, planeAxisDirection, planeAxisUp);

	m_splineBase = std::shared_ptr<BSpline>(new BSpline2D(planeAxisDirection, planeAxisUp, splineBaseControlPoints, splineBaseKnotValues));
	m_splineTop = std::shared_ptr<BSpline>(new BSpline2D(planeAxisDirection, planeAxisUp, splineBaseControlPoints, splineBaseKnotValues));
	
	calculateSplineTop(upToTopSpline);
}

glannotations::SplineState::SplineState(glm::vec3 position
	, glm::vec3 planeAxisDirection, glm::vec3 planeAxisUp
	, std::vector<glm::vec2> splineBaseControlPoints, unsigned int baseDegree
	, glm::vec2 upToTopSpline)
{
	initialize2D(position, planeAxisDirection, planeAxisUp);

	m_splineBase = std::shared_ptr<BSpline>(new BSpline2D(planeAxisDirection, planeAxisUp, splineBaseControlPoints, baseDegree));
	m_splineTop = std::shared_ptr<BSpline>(new BSpline2D(planeAxisDirection, planeAxisUp, splineBaseControlPoints, baseDegree));

	calculateSplineTop(upToTopSpline);
}

glannotations::SplineState::SplineState(glm::vec3 position
	, glm::vec3 planeAxisDirection, glm::vec3 planeAxisUp
	, std::vector<glm::vec2> splineBaseControlPoints, std::vector<float> splineBaseKnotValues
	, std::vector<glm::vec2> splineTopControlPoints, std::vector<float> splineTopKnotValues)
{
	initialize2D(position, planeAxisDirection, planeAxisUp);

	m_splineBase = std::shared_ptr<BSpline>(new BSpline2D(planeAxisDirection, planeAxisUp, splineBaseControlPoints, splineBaseKnotValues));
	m_splineTop = std::shared_ptr<BSpline>(new BSpline2D(planeAxisDirection, planeAxisUp, splineTopControlPoints, splineTopKnotValues));
}

glannotations::SplineState::SplineState(glm::vec3 position
	, glm::vec3 planeAxisDirection, glm::vec3 planeAxisUp
	, std::vector<glm::vec2> splineBaseControlPoints, unsigned int baseDegree
	, std::vector<glm::vec2> splineTopControlPoints, unsigned int topDegree)
{
	initialize2D(position, planeAxisDirection, planeAxisUp);

	m_splineBase = std::shared_ptr<BSpline>(new BSpline2D(planeAxisDirection, planeAxisUp, splineBaseControlPoints, baseDegree));
	m_splineTop = std::shared_ptr<BSpline>(new BSpline2D(planeAxisDirection, planeAxisUp, splineTopControlPoints, topDegree));
}

void glannotations::SplineState::initialize2D(glm::vec3 position, glm::vec3 planeAxisDirection, glm::vec3 planeAxisUp) {
	m_valid = true; //may be set false by following functions in constructor
	m_acceptsExternalReference = true;

	m_ll = position;
	m_lr = position + planeAxisDirection;
	m_ur = m_lr + planeAxisUp;
}

void glannotations::SplineState::initialize3D(glm::vec3 position) {
	m_valid = true; //may be set false by following functions in constructor
	m_acceptsExternalReference = false;
	
	m_ll = position;
	m_lr = position;
	m_ur = position;
}

void glannotations::SplineState::setExtends(glm::vec3 ll, glm::vec3 lr, glm::vec3 ur) {
	if (ll != m_ll || lr != m_lr || ur != m_ur) {
		setDirty(true);
		m_ll = ll;
		m_lr = lr;
		m_ur = ur;

		if (m_acceptsExternalReference) {
			glm::vec3 direction = m_lr - m_ll;
			glm::vec3 up = m_ur - m_lr;

			(m_splineBase->asBSpline2D()).setPlane(direction, up);
			(m_splineTop->asBSpline2D()).setPlane(direction, up);
		}
	}
}

bool glannotations::SplineState::isValid() const {
	return m_valid;
}

bool glannotations::SplineState::acceptsExternalReference() const {
	return m_acceptsExternalReference;
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

	//old splineTop_plane == new splineTop_plane?
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

const glm::vec3& glannotations::SplineState::getLR() const {
	return m_lr;
}

const glm::vec3& glannotations::SplineState::getUR() const {
	return m_ur;
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
	globjects::ref_ptr<glannotations::AbstractState> clonedState;
	if (m_acceptsExternalReference) {
		clonedState = globjects::ref_ptr<glannotations::AbstractState>(
			new SplineState(m_ll
			, (m_splineBase->asBSpline2D()).getPlaneDirection()
			, (m_splineBase->asBSpline2D()).getPlaneUp()
			, (m_splineBase->asBSpline2D()).getControlPoints2D(), m_splineBase->getKnotValues()
			, (m_splineTop->asBSpline2D()).getControlPoints2D(), m_splineTop->getKnotValues()
			)
		);
	}
	else {
		clonedState = globjects::ref_ptr<glannotations::AbstractState>(
			new SplineState(m_ll
			, m_splineBase->getControlPoints(), m_splineBase->getKnotValues()
			, m_splineTop->getControlPoints(), m_splineTop->getKnotValues()
			)
		);
	}
	
	ReferenceableState::copyState(dynamic_cast<ReferenceableState&> (*clonedState));
	return clonedState;
}

void glannotations::SplineState::setExternalReference(const globjects::ref_ptr<glannotations::AbstractExternalReference>& reference) {
	if (m_acceptsExternalReference) {
		ReferenceableState::setExternalReference(reference);
		m_externalReference->setupExternalReference(*this);
	}
	else {
		std::cerr << "Cannot set ExternalReference for BSpline3D. No reference set.";
	}
}

void glannotations::SplineState::updateExtends(glm::vec2 /*sourceExtends*/){
	//todo:anne
	if (m_acceptsExternalReference) {

		glm::vec3 direction = (m_splineBase->asBSpline2D()).getPlaneDirection();
		glm::vec3 up = (m_splineBase->asBSpline2D()).getPlaneUp();
		float width = glm::length(m_lr - m_ll);
		float height = glm::length(m_ur - m_lr);

		//cropExtends(m_ll, m_lr, m_ur, glm::vec2(width, height));

		(m_splineBase->asBSpline2D()).setPlane(direction * width, up * height);
	}
	
}

glannotations::BoundingBox glannotations::SplineState::getBoundingBox() {
	//todo: maybe there is a more efficient way
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

glm::vec4 glannotations::SplineState::getBoundingRect() {
	//todo: maybe there is a more efficient way

	if (!m_acceptsExternalReference) {
		return glm::vec4();
	}
		
	glm::vec4 brBase = (m_splineBase->asBSpline2D()).getBoundingRect();
	glm::vec4 brTop = (m_splineTop->asBSpline2D()).getBoundingRect();

	float minX = glm::min(brBase.x, brTop.x);
	float minY = glm::min(brBase.y, brTop.y);
	float maxX = glm::max(brBase.z, brTop.z);
	float maxY = glm::max(brBase.w, brTop.w);
	
	return glm::vec4(minX, minY, maxX, maxY);
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
