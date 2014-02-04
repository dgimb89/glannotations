#include <glat/DistanceFieldFontRenderer.h>
#include <glat/ViewportState.h>
#include <glat/InternalState.h>
#include <glat/FontAnnotation.h>
#include <glat/Outline.h>
#include <glat/BumpMap.h>
#include <glat/PNGImage.h>
#include <glat/GlyphSetConfig.h>
#include <glat/TextureManager.h>

const char* path = "glat_df.png";

using namespace glat;

void DistanceFieldFontRenderer::draw(AbstractAnnotation* annotation) {
	FontAnnotation* currentAnnotation = dynamic_cast<FontAnnotation*>(annotation);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	if (currentAnnotation->isDirty()) {
		std::vector<char> charCodes;
		for (unsigned i = 0; i < currentAnnotation->getText().length(); ++i) {
			charCodes.push_back(currentAnnotation->getText()[i]);
		}

		glat::GlyphSetConfig glyphSetConfig(currentAnnotation->getFontName());
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
	m_quad->setPosition(glm::vec3(state.getLLF(), 0.f), glm::vec3(state.getURB(), 0.f));
	m_quad->draw();
	glEnable(GL_DEPTH_TEST);
}


void DistanceFieldFontRenderer::drawSetupState(const InternalState& state) const {
	m_quad->setPosition(state.getLLF(), state.getURB(), state.getCamera()->viewProjection());
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


void DistanceFieldFontRenderer::setupOutline(Styling* outline) {
	if (outline == nullptr) return;
	Style::Outline* outlineStyle = reinterpret_cast<Style::Outline*>(outline);
	m_quad->setOutline(outlineStyle->getWidth()*0.01f, outlineStyle->getColor());
}


void DistanceFieldFontRenderer::setupBumpMap(Styling* bumpMap) {
	if (bumpMap == nullptr) return;
	Style::BumpMap* bumpMapStyle = reinterpret_cast<Style::BumpMap*>(bumpMap);
	m_quad->setBumpMap(bumpMapStyle->getIntensity());
}
