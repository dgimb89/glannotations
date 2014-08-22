#include <glbinding/gl/bitfield.h>
#include <glbinding/gl/functions.h>
#include <glbinding/gl/enum.h>
#include <glbinding/gl/types.h>

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
		annotation->setDirty(false);
	}
	m_currentText = currentAnnotation->getText().c_str();
	m_textColor = currentAnnotation->getColor();

	glat::NVPRRenderer::draw(annotation);
}

void glat::NVPRFontRenderer::drawSetupState(const glat::ViewportState& state) const {
	gl::glDisable(gl::GL_DEPTH_TEST);
	gl::GLfloat yMin, yMax;
	size_t messageLen = strlen(m_currentText);
	gl::GLfloat totalAdvance;
	gl::GLfloat* xtranslate = NULL;
	gl::GLfloat underline_position, underline_thickness;
	getTextStencelingDimensions(m_currentText, messageLen, xtranslate, totalAdvance, yMin, yMax, underline_position, underline_thickness);

	gl::glMatrixMode(gl::GL_PROJECTION);
	gl::glPushMatrix();
	gl::glLoadIdentity();

	// setting up an ortho projection for viewport placement
	setupOrthoProjection(state.getLL(), state.getUR(), totalAdvance, yMax - yMin, yMin);

	gl::glMatrixMode(gl::GL_MODELVIEW);
	gl::glPushMatrix();
	gl::glLoadIdentity();

	stencilThenCoverText(messageLen, xtranslate);
	if (m_drawOutline) {
		drawOutline(messageLen, xtranslate, state.getStyling("Outline"));
	}

	// cleanup
	gl::glPopMatrix();
	gl::glMatrixMode(gl::GL_PROJECTION);
	gl::glPopMatrix();
}

void glat::NVPRFontRenderer::drawSetupState(const glat::InternalState& state) const {
	gl::glEnable(gl::GL_DEPTH_TEST);
	gl::GLfloat yMin, yMax;
	size_t messageLen = strlen(m_currentText);
	gl::GLfloat totalAdvance;
	gl::GLfloat* xtranslate = NULL;
	gl::GLfloat underline_position, underline_thickness;
	getTextStencelingDimensions(m_currentText, messageLen, xtranslate, totalAdvance, yMin, yMax, underline_position, underline_thickness);

	gl::glMatrixMode(gl::GL_PROJECTION);
	gl::glPushMatrix();
	gl::glLoadIdentity();
	setupInternalProjection(state.getViewProjection(), state.getLL());

	gl::glMatrixMode(gl::GL_MODELVIEW);
	gl::glPushMatrix();
	gl::glLoadIdentity();
	setupInternalModelview(state.getLL(), state.getLR(), state.getUR(), totalAdvance, yMax - yMin);

	stencilThenCoverText(messageLen, xtranslate);
	if (m_drawOutline) {
		drawOutline(messageLen, xtranslate, state.getStyling("Outline"));
	}

	// cleanup
	gl::glPopMatrix();
	gl::glMatrixMode(gl::GL_PROJECTION);
	gl::glPopMatrix();
	}

void glat::NVPRFontRenderer::drawSetupState(const glat::PathState& state) const {
	throw std::logic_error("The method or operation is not implemented.");
}

void glat::NVPRFontRenderer::initializeFont(glat::FontAnnotation* annotation) {
	gl::GLuint pathSettings = ~0;
	gl::glPathCommandsNV(pathSettings, 0, NULL, 0, gl::GL_FLOAT, NULL);
	gl::glPathParameteriNV(pathSettings, gl::GL_PATH_JOIN_STYLE_NV, (gl::GLint) gl::GL_ROUND_NV);
	gl::glPathParameterfNV(pathSettings, gl::GL_PATH_MITER_LIMIT_NV, 1.0);
	setupOutline(pathSettings, annotation->getRenderState()->getStyling("Outline"), emScale*0.01f);

	/* Create a range of path objects corresponding to Latin-1 character codes. */
	gl::glDeletePathsNV(m_pathBase, numChars);
	m_pathBase = gl::glGenPathsNV(numChars);

	std::string fontfile = RESOURCES_DIR;
	fontfile.append(annotation->getFontName());

	gl::glPathGlyphRangeNV(m_pathBase,
		gl::GL_FILE_NAME_NV, fontfile.c_str(), gl::PathFontStyle(0),
		0, numChars,
		gl::GL_SKIP_MISSING_GLYPH_NV, pathSettings, emScale);

	// Fallback Standard font
	gl::glPathGlyphRangeNV(m_pathBase,
		gl::GL_STANDARD_FONT_NAME_NV, "Sans", gl::GL_BOLD_BIT_NV,
		0, numChars,
		gl::GL_USE_MISSING_GLYPH_NV, pathSettings, emScale);
}

inline gl::PathRenderingMaskNV operator|(gl::PathRenderingMaskNV a, gl::PathRenderingMaskNV b) {
	return static_cast<gl::PathRenderingMaskNV>(static_cast<int>(a) | static_cast<int>(b));
}

void glat::NVPRFontRenderer::getTextStencelingDimensions(const char* text, const size_t& messageLen, gl::GLfloat* &xtranslate, gl::GLfloat& totalAdvance, gl::GLfloat& yMin, gl::GLfloat& yMax,
	gl::GLfloat& underline_position, gl::GLfloat& underline_thickness) const {
	float font_data[4];
	gl::GLfloat horizontalAdvance[256];

	/* Query font and glyph metrics. */
	gl::glGetPathMetricRangeNV(gl::PathRenderingMaskNV::GL_FONT_Y_MIN_BOUNDS_BIT_NV | gl::GL_FONT_Y_MAX_BOUNDS_BIT_NV | gl::GL_FONT_UNDERLINE_POSITION_BIT_NV | gl::GL_FONT_UNDERLINE_THICKNESS_BIT_NV,
		m_pathBase + ' ', /*count*/1,
		4 * sizeof(gl::GLfloat),
		font_data);
	yMin = font_data[0];
	yMax = font_data[1];
	underline_position = font_data[2];
	underline_thickness = font_data[3];

	gl::glGetPathMetricRangeNV(gl::GL_GLYPH_HORIZONTAL_BEARING_ADVANCE_BIT_NV,
		m_pathBase, numChars,
		0, /* stride of zero means sizeof(GLfloat) since 1 bit in mask */
		horizontalAdvance);

	/* Query spacing information for example's message. */
	xtranslate = (gl::GLfloat*)malloc(2 * sizeof(gl::GLfloat)*messageLen);
	if (!xtranslate) {
		exit(1);
	}
	xtranslate[0] = 0;
	xtranslate[1] = 0;
	gl::glGetPathSpacingNV(gl::GL_ACCUM_ADJACENT_PAIRS_NV,
		(gl::GLsizei)messageLen, gl::GL_UNSIGNED_BYTE, m_currentText,
		m_pathBase,
		1.1, 1.0, gl::GL_TRANSLATE_2D_NV,
		xtranslate + 2);

	/* Total advance is accumulated spacing plus horizontal advance of
	the last glyph */
	totalAdvance = xtranslate[2 * (messageLen - 1)] + horizontalAdvance[m_currentText[messageLen - 1]];
}

void glat::NVPRFontRenderer::stencilThenCoverText(const size_t& messageLen, const gl::GLfloat* xtranslate) const {
	// fill
	gl::glStencilFillPathInstancedNV((gl::GLsizei)messageLen,
		gl::GL_UNSIGNED_BYTE, m_currentText, m_pathBase,
		gl::GL_PATH_FILL_MODE_NV, ~0,  /* Use all stencil bits */
		gl::GL_TRANSLATE_2D_NV, xtranslate);

	// cover
	gl::glColor4f(m_textColor.r, m_textColor.g, m_textColor.b, m_textColor.a);
	gl::glCoverFillPathInstancedNV((gl::GLsizei)messageLen,
		gl::GL_UNSIGNED_BYTE, m_currentText, m_pathBase,
		gl::GL_BOUNDING_BOX_OF_BOUNDING_BOXES_NV,
		gl::GL_TRANSLATE_2D_NV, xtranslate);
}

void glat::NVPRFontRenderer::drawOutline(const size_t& messageLen, const gl::GLfloat* xtranslate, const glat::Styling* outline) const {
	auto outlineStyle = reinterpret_cast<const glat::Styles::Outline*>(outline);
	gl::glStencilStrokePathInstancedNV((gl::GLsizei)messageLen,
		gl::GL_UNSIGNED_BYTE, m_currentText, m_pathBase,
		1, ~0,
		gl::GL_TRANSLATE_2D_NV, xtranslate);
	glm::vec3 outlineColor = outlineStyle->getColor();
	gl::glColor3f(outlineColor.r, outlineColor.g, outlineColor.b);
	gl::glCoverStrokePathInstancedNV((gl::GLsizei)messageLen,
		gl::GL_UNSIGNED_BYTE, m_currentText, m_pathBase,
		gl::GL_BOUNDING_BOX_OF_BOUNDING_BOXES_NV,
		gl::GL_TRANSLATE_2D_NV, xtranslate);
}

glat::NVPRFontRenderer::NVPRFontRenderer(gl::GLuint globalMatricesBindingIndex) : NVPRRenderer(globalMatricesBindingIndex) {

}
