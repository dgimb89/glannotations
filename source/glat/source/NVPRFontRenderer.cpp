#include <glat/NVPRFontRenderer.h>
#include <glat/FontAnnotation.h>
#include <glat/ViewportState.h>
#include <glat/InternalState.h>
#include <glat/Styles.h>
#include "glat-version.h"

void glat::NVPRFontRenderer::draw(const glow::ref_ptr<glat::AbstractAnnotation>& annotation) {
	glat::FontAnnotation* currentAnnotation = reinterpret_cast<glat::FontAnnotation*>(annotation.get());
	if (currentAnnotation->isDirty()) {
		clearStencilBuffer();
		initializeFont(currentAnnotation);
	}
	m_currentText = currentAnnotation->getText().c_str();
	m_textColor = currentAnnotation->getColor();

	glat::NVPRRenderer::draw(annotation);
}

void glat::NVPRFontRenderer::drawSetupState(const glat::ViewportState& state) const {
	glDisable(GL_DEPTH_TEST);
	GLfloat yMin, yMax;
	size_t messageLen = strlen(m_currentText);
	GLfloat totalAdvance;
	GLfloat* xtranslate = NULL;
	GLfloat underline_position, underline_thickness;
	getTextStencelingDimensions(m_currentText, messageLen, xtranslate, totalAdvance, yMin, yMax, underline_position, underline_thickness);

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();

	// setting up an ortho projection for viewport placement
	setupOrthoProjection(state.getLL(), state.getUR(), totalAdvance, yMax - yMin, yMin);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	stencilThenCoverText(messageLen, xtranslate);
	if (m_drawOutline) {
		drawOutline(messageLen, xtranslate, state.getStyling("Outline"));
	}

	// cleanup
	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
}

void glat::NVPRFontRenderer::drawSetupState(const glat::InternalState& state) const {
	glEnable(GL_DEPTH_TEST);
	GLfloat yMin, yMax;
	size_t messageLen = strlen(m_currentText);
	GLfloat totalAdvance;
	GLfloat* xtranslate = NULL;
	GLfloat underline_position, underline_thickness;
	getTextStencelingDimensions(m_currentText, messageLen, xtranslate, totalAdvance, yMin, yMax, underline_position, underline_thickness);

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	setupInternalProjection(state.getViewProjection(), state.getLL());

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	setupInternalModelview(state.getLL(), state.getLR(), state.getUR(), totalAdvance, yMax - yMin);

	stencilThenCoverText(messageLen, xtranslate);
	if (m_drawOutline) {
		drawOutline(messageLen, xtranslate, state.getStyling("Outline"));
	}

	// cleanup
	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();	
	}

void glat::NVPRFontRenderer::drawSetupState(const glat::InternalPathState& state) const {
	throw std::logic_error("The method or operation is not implemented.");
}

void glat::NVPRFontRenderer::initializeFont(glat::FontAnnotation* annotation) {
	GLuint pathSettings = ~0;
	glPathCommandsNV(pathSettings, 0, NULL, 0, GL_FLOAT, NULL);
	glPathParameteriNV(pathSettings, GL_PATH_JOIN_STYLE_NV, GL_MITER_TRUNCATE_NV);
	glPathParameterfNV(pathSettings, GL_PATH_MITER_LIMIT_NV, 1.0);
	setupOutline(pathSettings, annotation->getRenderState()->getStyling("Outline"), emScale*0.01f);

	/* Create a range of path objects corresponding to Latin-1 character codes. */
	glDeletePathsNV(m_pathBase, numChars);
	m_pathBase = glGenPathsNV(numChars);

	std::string fontfile = RESOURCES_DIR;
	fontfile.append(annotation->getFontName());

	glPathGlyphRangeNV(m_pathBase,
		GL_FILE_NAME_NV, fontfile.c_str(), 0,
		0, numChars,
		GL_SKIP_MISSING_GLYPH_NV, pathSettings, emScale);

	// Fallback Standard font
	glPathGlyphRangeNV(m_pathBase,
		GL_STANDARD_FONT_NAME_NV, "Sans", GL_BOLD_BIT_NV,
		0, numChars,
		GL_USE_MISSING_GLYPH_NV, pathSettings, emScale);
}

void glat::NVPRFontRenderer::getTextStencelingDimensions(const char* text, const size_t& messageLen, GLfloat* &xtranslate, GLfloat& totalAdvance, GLfloat& yMin, GLfloat& yMax,
		GLfloat& underline_position, GLfloat& underline_thickness) const {
	float font_data[4];
	GLfloat horizontalAdvance[256];

	/* Query font and glyph metrics. */
	glGetPathMetricRangeNV(GL_FONT_Y_MIN_BOUNDS_NV | GL_FONT_Y_MAX_BOUNDS_NV |
		GL_FONT_UNDERLINE_POSITION_NV | GL_FONT_UNDERLINE_THICKNESS_NV,
		m_pathBase + ' ', /*count*/1,
		4 * sizeof(GLfloat),
		font_data);
	yMin = font_data[0];
	yMax = font_data[1];
	underline_position = font_data[2];
	underline_thickness = font_data[3];

	glGetPathMetricRangeNV(GL_GLYPH_HORIZONTAL_BEARING_ADVANCE_BIT_NV,
		m_pathBase, numChars,
		0, /* stride of zero means sizeof(GLfloat) since 1 bit in mask */
		horizontalAdvance);

	/* Query spacing information for example's message. */
	xtranslate = (GLfloat*)malloc(2 * sizeof(GLfloat)*messageLen);
	if (!xtranslate) {
		exit(1);
	}
	xtranslate[0] = 0;
	xtranslate[1] = 0;
	glGetPathSpacingNV(GL_ACCUM_ADJACENT_PAIRS_NV,
		(GLsizei)messageLen, GL_UNSIGNED_BYTE, m_currentText,
		m_pathBase,
		1.1, 1.0, GL_TRANSLATE_2D_NV,
		xtranslate + 2);

	/* Total advance is accumulated spacing plus horizontal advance of
	the last glyph */
	totalAdvance = xtranslate[2 * (messageLen - 1)] + horizontalAdvance[m_currentText[messageLen - 1]];
}

void glat::NVPRFontRenderer::stencilThenCoverText(const size_t& messageLen, const GLfloat* xtranslate) const {
	// fill
	glStencilFillPathInstancedNV((GLsizei)messageLen,
		GL_UNSIGNED_BYTE, m_currentText, m_pathBase,
		GL_PATH_FILL_MODE_NV, ~0,  /* Use all stencil bits */
		GL_TRANSLATE_2D_NV, xtranslate);

	// cover
	glColor4f(m_textColor.r, m_textColor.g, m_textColor.b, m_textColor.a);
	glCoverFillPathInstancedNV((GLsizei)messageLen,
		GL_UNSIGNED_BYTE, m_currentText, m_pathBase,
		GL_BOUNDING_BOX_OF_BOUNDING_BOXES_NV,
		GL_TRANSLATE_2D_NV, xtranslate);
}

void glat::NVPRFontRenderer::drawOutline(const size_t& messageLen, const GLfloat* xtranslate, const glat::Styling* outline) const {
	auto outlineStyle = reinterpret_cast<const glat::Styles::Outline*>(outline);
	glStencilStrokePathInstancedNV((GLsizei)messageLen,
		GL_UNSIGNED_BYTE, m_currentText, m_pathBase,
		1, ~0,
		GL_TRANSLATE_2D_NV, xtranslate);
	glm::vec3 outlineColor = outlineStyle->getColor();
	glColor3f(outlineColor.r, outlineColor.g, outlineColor.b);
	glCoverStrokePathInstancedNV((GLsizei)messageLen,
		GL_UNSIGNED_BYTE, m_currentText, m_pathBase,
		GL_BOUNDING_BOX_OF_BOUNDING_BOXES_NV,
		GL_TRANSLATE_2D_NV, xtranslate);
}