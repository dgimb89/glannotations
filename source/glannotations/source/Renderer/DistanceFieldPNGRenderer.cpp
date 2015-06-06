#   define __STDC_LIMIT_MACROS

#include <glbinding/gl/bitfield.h>
#include <glbinding/gl/functions.h>

#include <glannotations/Renderer/DistanceFieldPNGRenderer.h>
#include <glannotations/States/ViewportState.h>
#include <glannotations/States/InternalState.h>
#include <glannotations/PNGAnnotation.h>
#include <glannotations/Common/TextureManager.h>
#include <glannotations/Renderer/QuadStrip.h>

void glannotations::DistanceFieldPNGRenderer::draw(const globjects::ref_ptr<glannotations::AbstractAnnotation>& annotation) {
	if (annotation->isDirty()) {
		prepare(annotation);
		//todo:anne
		//alles vorbereiten, was Text/Inhalt angeht
	}
	
	gl::glEnable(gl::GL_BLEND);
	gl::glBlendFunc(gl::GL_SRC_ALPHA, gl::GL_ONE_MINUS_SRC_ALPHA);
	annotation->getRenderState()->draw(*this);
	gl::glDisable(gl::GL_BLEND);
}

glannotations::DistanceFieldPNGRenderer::DistanceFieldPNGRenderer(gl::GLuint globalMatricesBindingIndex) : AbstractPrimitiveRenderer(globalMatricesBindingIndex) {

}

void glannotations::DistanceFieldPNGRenderer::prepare(const globjects::ref_ptr<glannotations::AbstractAnnotation>& annotation) {
	PNGAnnotation* currentAnnotation = dynamic_cast<PNGAnnotation*>(annotation.get());

	try {
		annotation->getRenderState()->asSplineState();
		auto bendedQuadStrip = new BendedQuadStrip(
			glannotations::TextureManager::getInstance().getTexture(currentAnnotation->getFileName()),
			m_globalMatricesBindingIndex,
			currentAnnotation->isDistanceField()
			);
		bendedQuadStrip->addQuad(glannotations::QuadStrip::texVec2_t(0.f, 0.f), glannotations::QuadStrip::texVec2_t(1.f, 1.f),
			glm::vec3(1, 0, 0), glm::vec3(0,1,0)
			);
		m_drawingPrimitive = bendedQuadStrip;
	}
	catch (...){ //Katastrophenschutz wegen bad_cast. todo:anne double dispatch oder irgendwas Sinnvolles
		auto quadstrip = new QuadStrip(
			glannotations::TextureManager::getInstance().getTexture(currentAnnotation->getFileName()),
			m_globalMatricesBindingIndex,
			currentAnnotation->isDistanceField()
			);
		quadstrip->addQuad(glannotations::QuadStrip::texVec2_t(0.f, 0.f), glannotations::QuadStrip::texVec2_t(1.f, 1.f));
		m_drawingPrimitive = quadstrip;
	}
	if (currentAnnotation->isDistanceField()) {
		m_drawingPrimitive->setColor(currentAnnotation->getColor());
		setupOutline(annotation->getRenderState()->getStyling("Outline"));
		setupBumpMap(annotation->getRenderState()->getStyling("BumpMap"));
	}
	annotation->setDirty(false);
}
