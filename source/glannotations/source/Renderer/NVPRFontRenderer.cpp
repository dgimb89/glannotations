#include <glbinding/gl/bitfield.h>
#include <glbinding/gl/functions.h>
#include <glbinding/gl/enum.h>
#include <glbinding/gl/types.h>

#include <glannotations/NVPRFontRenderer.h>
#include <glannotations/FontAnnotation.h>
#include <glannotations/ViewportState.h>
#include <glannotations/InternalState.h>
#include <glannotations/Styles.h>
#include "glannotations-version.h"

#include <string.h>

void glannotations::NVPRFontRenderer::draw(const globjects::ref_ptr<glannotations::AbstractAnnotation>& annotation) {
	if (annotation->isDirty()) {
		prepare(annotation);
	}
	glannotations::NVPRRenderer::draw(annotation);
}

void glannotations::NVPRFontRenderer::drawSetupState(glannotations::ViewportState& state) const {
	gl::glDisable(gl::GL_DEPTH_TEST);
	gl::GLfloat yMin, yMax;
	size_t messageLen = strlen(m_currentText);
	gl::GLfloat totalAdvance;
	gl::GLfloat* xtranslate = NULL;
	gl::GLfloat underline_position, underline_thickness;
	getTextStencelingDimensions(m_currentText, messageLen, xtranslate, totalAdvance, yMin, yMax, underline_position, underline_thickness);

	// setting up an ortho projection for viewport placement
	setupOrthoProjection(state.getLL(), state.getUR(), totalAdvance, yMax - yMin, yMin);
	pushEmptyModelViewMatrix();

	stencilThenCoverText(messageLen, xtranslate);
	if (m_drawOutline) {
		drawOutline(messageLen, xtranslate, state.getStyling("Outline"));
	}

	// cleanup
	cleanMatrixStacks();
	gl::glEnable(gl::GL_DEPTH_TEST);
}

void glannotations::NVPRFontRenderer::drawSetupState(glannotations::InternalState& state) const {
	gl::GLfloat yMin, yMax;
	size_t messageLen = strlen(m_currentText);
	gl::GLfloat totalAdvance;
	gl::GLfloat* xtranslate = NULL;
	gl::GLfloat underline_position, underline_thickness;
	getTextStencelingDimensions(m_currentText, messageLen, xtranslate, totalAdvance, yMin, yMax, underline_position, underline_thickness);
	
	setupProjection(glannotations::getProjection(getMatricesBindingIndex()));
	setupModelView(glannotations::getView(getMatricesBindingIndex()), state, totalAdvance, yMax - yMin);

	stencilThenCoverText(messageLen, xtranslate);
	if (m_drawOutline) {
		drawOutline(messageLen, xtranslate, state.getStyling("Outline"));
	}

	// cleanup
	cleanMatrixStacks();
	}

void glannotations::NVPRFontRenderer::drawSetupState(glannotations::SplineState& state) const {
	throw std::logic_error("The method or operation is not implemented.");
}

void glannotations::NVPRFontRenderer::initializeFont(glannotations::FontAnnotation* annotation) {
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

gl::PathRenderingMaskNV operator|(gl::PathRenderingMaskNV a, gl::PathRenderingMaskNV b) {
	return static_cast<gl::PathRenderingMaskNV>(static_cast<int>(a) | static_cast<int>(b));
}

void glannotations::NVPRFontRenderer::getTextStencelingDimensions(const char* text, const size_t& messageLen, gl::GLfloat* &xtranslate, gl::GLfloat& totalAdvance, gl::GLfloat& yMin, gl::GLfloat& yMax,
	gl::GLfloat& underline_position, gl::GLfloat& underline_thickness) const {
	float font_data[4];
	gl::GLfloat horizontalAdvance[256];

	/* Query font and glyph metrics. */
	auto flags = static_cast<gl::PathRenderingMaskNV>(
		(unsigned)gl::GL_FONT_Y_MIN_BOUNDS_BIT_NV | (unsigned)gl::GL_FONT_Y_MAX_BOUNDS_BIT_NV |
		(unsigned)gl::GL_FONT_UNDERLINE_POSITION_BIT_NV | (unsigned)gl::GL_FONT_UNDERLINE_THICKNESS_BIT_NV);
	gl::glGetPathMetricRangeNV(flags,
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

void glannotations::NVPRFontRenderer::stencilThenCoverText(const size_t& messageLen, const gl::GLfloat* xtranslate) const {
	gl::glPathCoverDepthFuncNV(gl::GL_ALWAYS);
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

void glannotations::NVPRFontRenderer::drawOutline(const size_t& messageLen, const gl::GLfloat* xtranslate, const glannotations::Styling* outline) const {
	auto outlineStyle = reinterpret_cast<const glannotations::Styles::Outline*>(outline);
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

glannotations::NVPRFontRenderer::NVPRFontRenderer(gl::GLuint globalMatricesBindingIndex) : NVPRRenderer(globalMatricesBindingIndex) {

}

void glannotations::NVPRFontRenderer::prepare(const globjects::ref_ptr<glannotations::AbstractAnnotation>& annotation) {
	glannotations::FontAnnotation* currentAnnotation = reinterpret_cast<glannotations::FontAnnotation*>(annotation.get());
	clearStencilBuffer();
	initializeFont(currentAnnotation);
	m_currentText = currentAnnotation->getText().c_str();
	m_textColor = currentAnnotation->getColor();
	annotation->setDirty(false);
}
