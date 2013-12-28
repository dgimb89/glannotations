#include <glat/DistanceFieldRenderer.h>
#include <glat/ViewportState.h>
#include <glat/FontAnnotation.h>
#include <glat/Outline.h>
#include <glat/BumpMap.h>

#include <png.h>
#include <iostream>

const char* path = "./distanceFieldCB.png";

using namespace glat;

void DistanceFieldRenderer::draw(AbstractAnnotation* annotation) {
	FontAnnotation* currentAnnotation = dynamic_cast<FontAnnotation*>(annotation);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	if (currentAnnotation->isDirty()) {
		char* image = loadDistanceField(path);
		glow::ref_ptr<glow::Texture> texture = createRGBATexture(image);
		m_quad = new Quad(texture);
		//m_quad->setScale(glm::vec2(4.0f, 4.5f));
		//m_quad->setOffset(glm::vec2(0.2f, 0.2f));
		
		setupOutline(annotation->getState()->getStyling("Outline"));
		setupBumpMap(annotation->getState()->getStyling("BumpMap"));
		currentAnnotation->setDirty(false);
	}

	annotation->getState()->draw(*this);

	glDisable(GL_BLEND);
}

void DistanceFieldRenderer::drawSetupState(const ViewportState& state) const {
	m_quad->draw();
}


glow::ref_ptr<glow::Texture> DistanceFieldRenderer::createRGBATexture(const char* image) {
	glow::ref_ptr<glow::Texture> texture = new glow::Texture(GL_TEXTURE_2D);
	texture->setParameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	texture->setParameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	texture->setParameter(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	texture->setParameter(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	texture->setParameter(GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	texture->image2D(0, GL_R8, m_width, m_height, 0, GL_RED, GL_UNSIGNED_BYTE, image);
	return texture;
}


void DistanceFieldRenderer::setupOutline(Styling* outline) {
	if (outline == nullptr) return;
	Style::Outline* outlineStyle = reinterpret_cast<Style::Outline*>(outline);
	m_quad->setOutline(outlineStyle->getWidth()*0.01f, outlineStyle->getColor());
}

void DistanceFieldRenderer::setupBumpMap(Styling* bumpMap) {
	if (bumpMap == nullptr) return;
	Style::BumpMap* bumpMapStyle = reinterpret_cast<Style::BumpMap*>(bumpMap);
	m_quad->setBumpMap(bumpMapStyle->getIntensity());
}
