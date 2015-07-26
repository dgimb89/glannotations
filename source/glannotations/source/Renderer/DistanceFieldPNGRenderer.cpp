#   define __STDC_LIMIT_MACROS

#include <glbinding/gl/bitfield.h>
#include <glbinding/gl/functions.h>

#include <glannotations/Renderer/DistanceFieldPNGRenderer.h>
#include <glannotations/States/ViewportState.h>
#include <glannotations/States/QuadState.h>
#include <glannotations/PNGAnnotation.h>
#include <glannotations/Common/TextureManager.h>
#include <glannotations/Renderer/QuadStrip.h>

void glannotations::DistanceFieldPNGRenderer::draw(const globjects::ref_ptr<glannotations::AbstractAnnotation>& annotation) {
	if (annotation->isDirty()) {
		prepare(annotation);
	}
	
	gl::glEnable(gl::GL_BLEND);
	gl::glBlendFunc(gl::GL_SRC_ALPHA, gl::GL_ONE_MINUS_SRC_ALPHA);
	annotation->getRenderState()->draw(annotation, *this);
	gl::glDisable(gl::GL_BLEND);
}

glannotations::DistanceFieldPNGRenderer::DistanceFieldPNGRenderer(gl::GLuint globalMatricesBindingIndex) : AbstractPrimitiveRenderer(globalMatricesBindingIndex) {

}

void glannotations::DistanceFieldPNGRenderer::prepare(const globjects::ref_ptr<glannotations::AbstractAnnotation>& annotation) {
	PNGAnnotation* currentAnnotation = dynamic_cast<PNGAnnotation*>(annotation.get());
	m_texture = glannotations::TextureManager::getInstance().getTexture(currentAnnotation->getFileName());
}

void glannotations::DistanceFieldPNGRenderer::drawSetupState(const globjects::ref_ptr<glannotations::AbstractAnnotation>& annotation, glannotations::ViewportState& state) const  {
	if (annotation->isDirty()) {
		prepareQuadStrip(dynamic_cast<PNGAnnotation*>(annotation.get()));
	}
	AbstractPrimitiveRenderer::drawSetupState(annotation, state);
}

void glannotations::DistanceFieldPNGRenderer::drawSetupState(const globjects::ref_ptr<glannotations::AbstractAnnotation>& annotation, glannotations::QuadState& state) const  {
	if (annotation->isDirty()) {
		prepareQuadStrip(dynamic_cast<PNGAnnotation*>(annotation.get()));
	}
	AbstractPrimitiveRenderer::drawSetupState(annotation, state);
}

void glannotations::DistanceFieldPNGRenderer::drawSetupState(const globjects::ref_ptr<glannotations::AbstractAnnotation>& annotation, glannotations::SplineState& state) const  {
	if (annotation->isDirty()) {
		prepareSpline(dynamic_cast<PNGAnnotation*>(annotation.get()));
	}
	AbstractPrimitiveRenderer::drawSetupState(annotation, state);
}

void glannotations::DistanceFieldPNGRenderer::setupStylings(PNGAnnotation* annotation) const {
	if (annotation->isDistanceField()) {
		m_drawingPrimitive->setColor(annotation->getColor());
		setupOutline(annotation->getRenderState()->getStyling("Outline"));
		setupBumpMap(annotation->getRenderState()->getStyling("BumpMap"));
	}
}

void glannotations::DistanceFieldPNGRenderer::prepareQuadStrip(PNGAnnotation* annotation) const {
	auto quadstrip = new QuadStrip(m_texture, m_globalMatricesBindingIndex, annotation->isDistanceField());
	quadstrip->addQuad(glannotations::QuadStrip::texVec2_t(0.f, 0.f), glannotations::QuadStrip::texVec2_t(1.f, 1.f));
	m_drawingPrimitive = quadstrip;
	setupStylings(annotation);
	annotation->setDirty(false);
}

void glannotations::DistanceFieldPNGRenderer::prepareSpline(PNGAnnotation* annotation) const {
	auto bendedQuadStrip = new SplineQuads(m_texture, m_globalMatricesBindingIndex, annotation->isDistanceField());

	auto firstPoint = (annotation->getRenderState()->asSplineState()).retrieveStartingPoint();
	bendedQuadStrip->setFirstPoint(firstPoint);

	bendedQuadStrip->addQuad(glannotations::QuadStrip::texVec2_t(0.f, 0.f), glannotations::QuadStrip::texVec2_t(1.f, 1.f),
		glm::vec3(1, 0, 0), glm::vec3(0, 1, 0)
		);
	m_drawingPrimitive = bendedQuadStrip;
	setupStylings(annotation);
	annotation->setDirty(false);
}

void glannotations::DistanceFieldPNGRenderer::directDrawCall() {
	m_drawingPrimitive->directDrawCall();
}
