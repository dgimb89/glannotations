#include <glat/DistanceFieldFontRenderer.h>
#include <glat/ViewportState.h>
#include <glat/InternalState.h>
#include <glat/FontAnnotation.h>
#include <glat/Outline.h>
#include <glat/BumpMap.h>
#include <glat/PNGImage.h>
#include <glat/TextureManager.h>

const char* path = "glat_df.png";

using namespace glat;

void DistanceFieldFontRenderer::draw(AbstractAnnotation* annotation) {
	FontAnnotation* currentAnnotation = dynamic_cast<FontAnnotation*>(annotation);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	if (currentAnnotation->isDirty()) {
		m_glyphConfig = new glat::GlyphSetConfig(currentAnnotation->getFontName());
		std::vector<char> charCodes;
		for (unsigned i = 0; i < currentAnnotation->getText().length(); ++i) {
			charCodes.push_back(currentAnnotation->getText()[i]);
		}
		//glyphSetConfig.getGlyphConfigForCharcode(charCodes.back());
		m_quad = new Quad(glat::TextureManager::getInstance()->getTexture(std::string(path)));

		setupOutline(annotation->getState()->getStyling("Outline"));
		setupBumpMap(annotation->getState()->getStyling("BumpMap"));
		currentAnnotation->setDirty(false);
	}

	m_quad->setTextColor(currentAnnotation->getColor());
	annotation->getState()->draw(*this);

	glDisable(GL_BLEND);
}

void DistanceFieldFontRenderer::drawSetupState(const ViewportState& state) const {
	glDisable(GL_DEPTH_TEST);
	if (state.isDirty()) {
		m_quad->setPosition(glm::vec3(state.getLL(), 0.0), glm::vec3(state.getLR(), 0.0), glm::vec3(state.getUR(), 0.0));
		state.setDirty(false);
	}
	m_quad->draw();
	glEnable(GL_DEPTH_TEST);
}


void DistanceFieldFontRenderer::drawSetupState(const InternalState& state) const {
	if (state.isDirty()) {
		m_quad->setPosition(state.getLL(), state.getLR(), state.getUR(), state.getViewProjection());
	}
	m_quad->draw();
}


glow::ref_ptr<glow::Texture> DistanceFieldFontRenderer::createRGBATexture(std::string distanceFieldFile) {
	glow::ref_ptr<glow::Texture> texture = new glow::Texture(GL_TEXTURE_2D);
	texture->setParameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	texture->setParameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	texture->setParameter(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	texture->setParameter(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	texture->setParameter(GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	glat::PNGImage dfImage(distanceFieldFile);
	/*dfImage.distanceTransform();
	dfImage.scaleToHeight(196);*/
	texture->image2D(0, GL_RED, dfImage.getWidth(), dfImage.getHeight(), 0, GL_RED, GL_UNSIGNED_BYTE, dfImage.getImage()->data);
	return texture;
}


void DistanceFieldFontRenderer::setupOutline(const Styling* outline) {
	if (outline == nullptr) return;
	const Style::Outline* outlineStyle = reinterpret_cast<const Style::Outline*>(outline);
	m_quad->setOutline(outlineStyle->getWidth()*0.01f, outlineStyle->getColor());
}


void DistanceFieldFontRenderer::setupBumpMap(const Styling* bumpMap) {
	if (bumpMap == nullptr) return;
	const Style::BumpMap* bumpMapStyle = reinterpret_cast<const Style::BumpMap*>(bumpMap);
	m_quad->setBumpMap(bumpMapStyle->getIntensity());
}
