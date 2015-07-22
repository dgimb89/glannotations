#include <glannotations/States/SplineState.h>
#include <glannotations/Renderer/AbstractRenderer.h>
#include <glannotations/AbstractAnnotation.h>
#include <glannotations/States/StateInterpolation.h>

#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/vector_angle.hpp>

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
	, glm::vec3 planeAxisRight, glm::vec3 planeAxisUp
	, std::vector<glm::vec2> splineBaseControlPoints, std::vector<float> splineBaseKnotValues
	, glm::vec2 upToTopSpline)
{
	initialize2D(position, planeAxisRight, planeAxisUp);

	m_splineBase = std::shared_ptr<BSpline>(new BSpline2D(splineBaseControlPoints, splineBaseKnotValues));
	m_splineTop = std::shared_ptr<BSpline>(new BSpline2D(splineBaseControlPoints, splineBaseKnotValues));
	
	calculateSplineTop(upToTopSpline);

	calculateTransformationMatrix();
}

glannotations::SplineState::SplineState(glm::vec3 position
	, glm::vec3 planeAxisRight, glm::vec3 planeAxisUp
	, std::vector<glm::vec2> splineBaseControlPoints, unsigned int baseDegree
	, glm::vec2 upToTopSpline)
{
	initialize2D(position, planeAxisRight, planeAxisUp);

	m_splineBase = std::shared_ptr<BSpline>(new BSpline2D(splineBaseControlPoints, baseDegree));
	m_splineTop = std::shared_ptr<BSpline>(new BSpline2D(splineBaseControlPoints, baseDegree));

	calculateSplineTop(upToTopSpline);

	calculateTransformationMatrix();
}

glannotations::SplineState::SplineState(glm::vec3 position
	, glm::vec3 planeAxisRight, glm::vec3 planeAxisUp
	, std::vector<glm::vec2> splineBaseControlPoints, std::vector<float> splineBaseKnotValues
	, std::vector<glm::vec2> splineTopControlPoints, std::vector<float> splineTopKnotValues)
{
	initialize2D(position, planeAxisRight, planeAxisUp);

	m_splineBase = std::shared_ptr<BSpline>(new BSpline2D(splineBaseControlPoints, splineBaseKnotValues));
	m_splineTop = std::shared_ptr<BSpline>(new BSpline2D(splineTopControlPoints, splineTopKnotValues));

	calculateTransformationMatrix();
}

glannotations::SplineState::SplineState(glm::vec3 position
	, glm::vec3 planeAxisRight, glm::vec3 planeAxisUp
	, std::vector<glm::vec2> splineBaseControlPoints, unsigned int baseDegree
	, std::vector<glm::vec2> splineTopControlPoints, unsigned int topDegree)
{
	initialize2D(position, planeAxisRight, planeAxisUp);

	m_splineBase = std::shared_ptr<BSpline>(new BSpline2D(splineBaseControlPoints, baseDegree));
	m_splineTop = std::shared_ptr<BSpline>(new BSpline2D(splineTopControlPoints, topDegree));

	calculateTransformationMatrix();
}

void glannotations::SplineState::initialize2D(glm::vec3 position, glm::vec3 planeAxisRight, glm::vec3 planeAxisUp) {
	m_valid = true; //may be set false by following functions in constructor
	m_acceptsExternalReference = true;

	m_ll = position;
	m_lr = position + planeAxisRight;
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
	}
}

bool glannotations::SplineState::isValid() const {
	return m_valid;
}

bool glannotations::SplineState::acceptsExternalReference() const {
	return m_acceptsExternalReference;
}

inline std::vector<glm::vec2> discardZ(const std::vector<glm::vec3> points) {
	std::vector<glm::vec2> points2D;

	for (int i = 0; i < points.size(); i++) {
		points2D.push_back(glm::vec2(points[i].x, points[i].y));
	}

	return points2D;
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

	std::vector<glm::vec3> ctrlPoints = splineTop->getControlPoints();
	//discard z
	std::vector<glm::vec2> ctrlPoints2D = discardZ(ctrlPoints);

	(m_splineTop->asBSpline2D()).setControlPoints(ctrlPoints2D);
	m_splineTop->setKnotValues(splineTop->getKnotValues());
	m_splineTop->calculateSplineDegree();
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

	std::vector<glm::vec2> ctrlPoints2D;

	if (m_acceptsExternalReference){
		auto ctrlPoints = (m_splineBase->asBSpline2D()).getControlPoints();
		for (size_t i = 0; i < ctrlPoints.size(); i++) {
			ctrlPoints2D.push_back(glm::vec2(
				ctrlPoints[i].x + upVecInPlaneSpace.x,
				ctrlPoints[i].y + upVecInPlaneSpace.y
				)
			);
		}

		(m_splineTop->asBSpline2D()).setControlPoints(ctrlPoints2D);

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

		auto ctrlPointsBase = discardZ((m_splineBase->asBSpline2D()).getControlPoints());
		auto ctrlPointsTop = discardZ((m_splineTop->asBSpline2D()).getControlPoints());

		clonedState = globjects::ref_ptr<glannotations::AbstractState>(
			new SplineState(m_ll
			, m_lr - m_ll
			, m_ur - m_lr
			, ctrlPointsBase
			, m_splineBase->getKnotValues()
			, ctrlPointsTop
			, m_splineTop->getKnotValues()
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
		std::cerr << "#Warning: Cannot set ExternalReference for BSpline3D. No reference set.\n";
	}
}

void glannotations::SplineState::updateExtends(glm::vec2 sourceExtends){
	if (m_acceptsExternalReference) {

		cropExtends(m_ll, m_lr, m_ur, sourceExtends);

		calculateTransformationMatrix();
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

glm::vec4 glannotations::SplineState::getBoundingRect() const {
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

glm::mat4 glannotations::SplineState::getTransformationMatrix() const {
	return m_transformation;
}

void glannotations::SplineState::calculateTransformationMatrix() {
		
	//no translation, because SplineState will take care of positioning later

	//scaling matrix
	glm::mat4 scalingM = glm::mat4();

	glm::vec3 right = m_lr - m_ll;
	glm::vec3 up = m_ur - m_lr;
	float width = glm::length(right);
	float height = glm::length(up);
		
	glm::vec4 plane = getBoundingRect();
	float scaleX = width / (plane.z - plane.x);
	float scaleY = height / (plane.w - plane.y);
	scalingM = glm::scale(scalingM, glm::vec3(scaleX, scaleY, 1.f));

	//rotation matrix to 
	auto eye = glm::vec3(0, 0, 0);
	auto center = glm::normalize(glm::cross(glm::normalize(up), glm::normalize(right)));

	glm::mat4 rotation = glm::lookAt(eye, eye + center, up);
	rotation = glm::inverse(rotation);

	m_transformation = rotation * scalingM;
}

void glannotations::SplineState::setSplineDirty(bool dirty) {
	m_splineBase->setDirty(dirty);
	m_splineTop->setDirty(dirty);
}

bool glannotations::SplineState::isSplineDirty() const {
	return (m_splineBase->isDirty() || m_splineTop->isDirty());
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
