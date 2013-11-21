#include <glat/NVPRFontRenderer.h>
#include "nvpr_init.hpp"
#include <glat/AbstractAnnotation.h>

const char *message = "OpenGL Annotations Toolkit! - NV_Path_Rendering";

void glat::NVPRFontRenderer::render(glat::AbstractAnnotation* annotation) {
	if (annotation->isDirty()) {
		glClearStencil(0);
		glStencilFunc(GL_NOTEQUAL, 0, 0x1F);
		glStencilOp(GL_KEEP, GL_KEEP, GL_ZERO);
		initializeFont();
		annotation->setDirty(false);
	}
	// disable depth test and back face culling
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);

	// enable stencil test as needed by nvpr
	glEnable(GL_STENCIL_TEST);

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();

	// hard coded 2d viewport rendering as of yet
	int viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);
	glOrtho(viewport[0], viewport[2], viewport[3], viewport[1], -1.0, 10.0);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	glColor3f(1, 0, 0);

	// render annotations here
	glStencilFillPathInstancedNV((GLsizei)messageLen,
		GL_UNSIGNED_BYTE, message, m_glyphBase,
		GL_PATH_FILL_MODE_NV, ~0,  /* Use all stencil bits */
		GL_TRANSLATE_2D_NV, xtranslate);
	glColor3ub(192, 192, 192);  // gray
	glCoverFillPathInstancedNV((GLsizei)messageLen,
		GL_UNSIGNED_BYTE, message, m_glyphBase,
		GL_BOUNDING_BOX_OF_BOUNDING_BOXES_NV,
		GL_TRANSLATE_2D_NV, xtranslate);

	// cleanup
	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();

	// disable stencil test when finished
	glDisable(GL_STENCIL_TEST);
}

void glat::NVPRFontRenderer::initializeFont() {
	const unsigned char *message_ub = (const unsigned char*)"OpenGL Annotations Toolkit!";
	float font_data[4];
	const int numChars = 256;  // ISO/IEC 8859-1 8-bit character range
	GLfloat horizontalAdvance[256];

	//glDeletePathsNV(m_glyphBase, numChars);

	m_pathTemplate = ~0;
	glPathCommandsNV(m_pathTemplate, 0, NULL, 0, GL_FLOAT, NULL);
	glPathParameterfNV(m_pathTemplate, GL_PATH_STROKE_WIDTH_NV, emScale*0.1f);
	glPathParameteriNV(m_pathTemplate, GL_PATH_JOIN_STYLE_NV, GL_MITER_TRUNCATE_NV);
	glPathParameterfNV(m_pathTemplate, GL_PATH_MITER_LIMIT_NV, 1.0);

	/* Create a range of path objects corresponding to Latin-1 character codes. */
	m_glyphBase = glGenPathsNV(numChars);
	/* Choose a bold sans-serif font face, preferring Veranda over Arial; if
	neither font is available as a system font, settle for the "Sans" standard
	(built-in) font. */
	glPathGlyphRangeNV(m_glyphBase,
		GL_SYSTEM_FONT_NAME_NV, "Verdana", GL_BOLD_BIT_NV,
		0, numChars,
		GL_USE_MISSING_GLYPH_NV, m_pathTemplate,
		emScale);
	glPathGlyphRangeNV(m_glyphBase,
		GL_SYSTEM_FONT_NAME_NV, "Arial", GL_BOLD_BIT_NV,
		0, numChars,
		GL_USE_MISSING_GLYPH_NV, m_pathTemplate,
		emScale);
	glPathGlyphRangeNV(m_glyphBase,
		GL_STANDARD_FONT_NAME_NV, "Sans", GL_BOLD_BIT_NV,
		0, numChars,
		GL_USE_MISSING_GLYPH_NV, m_pathTemplate,
		emScale);

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
	messageLen = strlen(message);
	free(xtranslate);
	xtranslate = (GLfloat*)malloc(2 * sizeof(GLfloat)*messageLen);
	if (!xtranslate) {
		exit(1);
	}
	xtranslate[0] = 0;
	xtranslate[1] = 0;
	glGetPathSpacingNV(GL_ACCUM_ADJACENT_PAIRS_NV,
		(GLsizei)messageLen, GL_UNSIGNED_BYTE, message,
		m_glyphBase,
		1.1, 1.0, GL_TRANSLATE_2D_NV,
		xtranslate + 2);

	/* Total advance is accumulated spacing plus horizontal advance of
	the last glyph */
	totalAdvance = xtranslate[2 * (messageLen - 1)] +
		horizontalAdvance[message_ub[messageLen - 1]];
	initialShift = totalAdvance / messageLen;

	glEnable(GL_STENCIL_TEST);
	glStencilFunc(GL_NOTEQUAL, 0, ~0);
	glStencilOp(GL_KEEP, GL_KEEP, GL_ZERO);
}

glat::NVPRFontRenderer::NVPRFontRenderer()
{
	initializeNVPR();
}
