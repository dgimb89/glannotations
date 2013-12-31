#include <glat/NVPRFontRenderer.h>
#include <glat/FontAnnotation.h>
#include <glat/ViewportState.h>
#include <glat/Outline.h>

void glat::NVPRFontRenderer::draw(glat::AbstractAnnotation* annotation) {
	glat::FontAnnotation* currentAnnotation = reinterpret_cast<glat::FontAnnotation*>(annotation);
	if (currentAnnotation->isDirty()) {
		clearStencilBuffer();
		glStencilFunc(GL_NOTEQUAL, 0, 0x1F);
		glStencilOp(GL_KEEP, GL_KEEP, GL_ZERO);
		initializeFont(currentAnnotation);
		currentAnnotation->setDirty(false);
	}
	m_currentText = currentAnnotation->getText().c_str();
	m_textColor = currentAnnotation->getColor();
	glat::NVPRRenderer::draw(annotation);
}

void glat::NVPRFontRenderer::drawSetupState(const glat::ViewportState& state) const {
	float font_data[4];
	GLfloat horizontalAdvance[256];
	GLfloat yMin, yMax;
	size_t messageLen = strlen(m_currentText);
	GLfloat totalAdvance;
	GLfloat* xtranslate = NULL;
	GLfloat underline_position, underline_thickness;
	/* Query font and glyph metrics. */
	glGetPathMetricRangeNV(GL_FONT_Y_MIN_BOUNDS_NV | GL_FONT_Y_MAX_BOUNDS_NV |
		GL_FONT_UNDERLINE_POSITION_NV | GL_FONT_UNDERLINE_THICKNESS_NV,
		m_glyphBase + ' ', /*count*/1,
		4 * sizeof(GLfloat),
		font_data);
	yMin = font_data[0];
	yMax = font_data[1];
	underline_position = font_data[2];
	underline_thickness = font_data[3];

	glGetPathMetricRangeNV(GL_GLYPH_HORIZONTAL_BEARING_ADVANCE_BIT_NV,
		m_glyphBase, numChars,
		0, /* stride of zero means sizeof(GLfloat) since 1 bit in mask */
		horizontalAdvance);

	/* Query spacing information for example's message. */
	free(xtranslate);
	xtranslate = (GLfloat*)malloc(2 * sizeof(GLfloat)*messageLen);
	if (!xtranslate) {
		exit(1);
	}
	xtranslate[0] = 0;
	xtranslate[1] = 0;
	glGetPathSpacingNV(GL_ACCUM_ADJACENT_PAIRS_NV,
		(GLsizei)messageLen, GL_UNSIGNED_BYTE, m_currentText,
		m_glyphBase,
		1.1, 1.0, GL_TRANSLATE_2D_NV,
		xtranslate + 2);

	/* Total advance is accumulated spacing plus horizontal advance of
	the last glyph */
	totalAdvance = xtranslate[2 * (messageLen - 1)] +
		horizontalAdvance[m_currentText[messageLen - 1]];

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();

	// hard coded 2d viewport rendering as of yet
	/*int viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);
	float aspect_ratio = viewport[2] / viewport[3];
	float scale = 1.f;
	glOrtho((-initialShift*5)*scale,
	(totalAdvance + initialShift*5)*scale,
	(-0.1*totalAdvance*aspect_ratio + (yMax + yMin) / 2)*scale,
	(0.1*totalAdvance*aspect_ratio + (yMax + yMin) / 2)*scale,
	-1, 1);*/

	setupOrthoProjection(state.getLLF(), state.getURB(), totalAdvance, yMax - yMin, yMin);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	glColor3f(1, 0, 0);

	//outline 
	if (m_drawOutline) {
		glStencilStrokePathInstancedNV((GLsizei)messageLen,
			GL_UNSIGNED_BYTE, m_currentText, m_glyphBase,
			1, ~0,
			GL_TRANSLATE_2D_NV, xtranslate);
		glm::vec3 outlineColor = reinterpret_cast<glat::Style::Outline*>(state.getStyling("Outline").get())->getColor();
		glColor3f(outlineColor.r, outlineColor.g, outlineColor.b);
		glCoverStrokePathInstancedNV((GLsizei)messageLen,
			GL_UNSIGNED_BYTE, m_currentText, m_glyphBase,
			GL_BOUNDING_BOX_OF_BOUNDING_BOXES_NV,
			GL_TRANSLATE_2D_NV, xtranslate);
	}

	// fill
	glStencilFillPathInstancedNV((GLsizei)messageLen,
		GL_UNSIGNED_BYTE, m_currentText, m_glyphBase,
		GL_PATH_FILL_MODE_NV, ~0,  /* Use all stencil bits */
		GL_TRANSLATE_2D_NV, xtranslate);
	//glColor3ub(192, 192, 192);  // dark gray
	glColor4f(m_textColor.r, m_textColor.g, m_textColor.b, m_textColor.a);
	glCoverFillPathInstancedNV((GLsizei)messageLen,
		GL_UNSIGNED_BYTE, m_currentText, m_glyphBase,
		GL_BOUNDING_BOX_OF_BOUNDING_BOXES_NV,
		GL_TRANSLATE_2D_NV, xtranslate);

	// cleanup
	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
}

void glat::NVPRFontRenderer::initializeFont(glat::FontAnnotation* annotation) {
	m_pathSettings = ~0;
	glPathCommandsNV(m_pathSettings, 0, NULL, 0, GL_FLOAT, NULL);
	glPathParameteriNV(m_pathSettings, GL_PATH_JOIN_STYLE_NV, GL_MITER_TRUNCATE_NV);
	glPathParameterfNV(m_pathSettings, GL_PATH_MITER_LIMIT_NV, 1.0);
	setupOutline(annotation->getState()->getStyling("Outline"), emScale*0.01f);

	/* Create a range of path objects corresponding to Latin-1 character codes. */
	m_glyphBase = glGenPathsNV(numChars);
	/* Choose a bold sans-serif font face, preferring Veranda over Arial; if
	neither font is available as a system font, settle for the "Sans" standard
	(built-in) font. */
	glPathGlyphRangeNV(m_glyphBase,
		GL_SYSTEM_FONT_NAME_NV, "Calibri", GL_BOLD_BIT_NV,
		0, numChars,
		GL_USE_MISSING_GLYPH_NV, m_pathSettings,
		emScale);
	glPathGlyphRangeNV(m_glyphBase,
		GL_SYSTEM_FONT_NAME_NV, "Arial", GL_BOLD_BIT_NV,
		0, numChars,
		GL_USE_MISSING_GLYPH_NV, m_pathSettings,
		emScale);
	glPathGlyphRangeNV(m_glyphBase,
		GL_STANDARD_FONT_NAME_NV, "Sans", GL_BOLD_BIT_NV,
		0, numChars,
		GL_USE_MISSING_GLYPH_NV, m_pathSettings,
		emScale);
}