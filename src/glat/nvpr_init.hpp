/* Copyright NVIDIA Corporation, 2010. */
// This software contains source code provided by NVIDIA Corporation.

#define NOMINMAX
#include <Windows.h>
#include <gl/GL.h>

#ifndef GLAPIENTRY
# ifdef _WIN32
#  define GLAPIENTRYP __stdcall *
# else
#  define GLAPIENTRYP *
# endif
#endif

#ifndef GL_NV_path_rendering
/* Tokens */
#define GL_STANDARD_FONT_NAME_NV                            0x9072
#define GL_SYSTEM_FONT_NAME_NV                              0x9073
#define GL_PATH_FILL_MODE_NV                                0x9080
#define GL_TRANSLATE_X_NV                                   0x908E
#define GL_BOUNDING_BOX_OF_BOUNDING_BOXES_NV                0x909C
#define GL_BOLD_BIT_NV                                      0x01
#define GL_ITALIC_BIT_NV                                    0x02
#define GL_GLYPH_HORIZONTAL_BEARING_ADVANCE_BIT_NV          0x10
#define GL_FONT_Y_MIN_BOUNDS_NV                             0x00020000
#define GL_FONT_Y_MAX_BOUNDS_NV                             0x00080000
#define GL_FONT_UNDERLINE_POSITION_NV                       0x04000000
#define GL_FONT_UNDERLINE_THICKNESS_NV                      0x08000000
#define GL_ACCUM_ADJACENT_PAIRS_NV                          0x90AD
#define GL_PATH_STROKE_WIDTH_NV                             0x9075
#define GL_PATH_JOIN_STYLE_NV                               0x9079
#define GL_MITER_TRUNCATE_NV                                0x90A8
#define GL_PATH_MITER_LIMIT_NV                              0x907A
#define GL_PRIMARY_COLOR                                    0x8577
#define GL_PATH_OBJECT_BOUNDING_BOX_NV                      0x908A
#define GL_USE_MISSING_GLYPH_NV                             0x90AA
/* Command and query function types */
typedef GLint(GLAPIENTRYP PFNGLGENPATHSNVPROC) (GLsizei range);
typedef void (GLAPIENTRYP PFNGLDELETEPATHSNVPROC) (GLuint path, GLsizei range);
typedef void (GLAPIENTRYP PFNGLPATHCOMMANDSNVPROC) (GLuint path, GLsizei numCommands, const GLubyte *commands, GLsizei numCoords, GLenum coordType, const GLvoid *coords);
typedef void (GLAPIENTRYP PFNGLPATHGLYPHRANGENVPROC) (GLuint firstPathName, GLenum fontTarget, const GLvoid *fontName, GLbitfield fontStyle, GLuint firstGlyph, GLsizei numGlyphs, GLenum handleMissingGlyphs, GLuint pathParameterTemplate, GLfloat emScale);
typedef void (GLAPIENTRYP PFNGLSTENCILFILLPATHINSTANCEDNVPROC) (GLsizei numPaths, GLenum pathNameType, const GLvoid *paths, GLuint pathBase, GLenum fillMode, GLuint mask, GLenum transformType, const GLfloat *transformValues);
typedef void (GLAPIENTRYP PFNGLSTENCILSTROKEPATHINSTANCEDNVPROC) (GLsizei numPaths, GLenum pathNameType, const GLvoid *paths, GLuint pathBase, GLint reference, GLuint mask, GLenum transformType, const GLfloat *transformValues);
typedef void (GLAPIENTRYP PFNGLCOVERFILLPATHINSTANCEDNVPROC) (GLsizei numPaths, GLenum pathNameType, const GLvoid *paths, GLuint pathBase, GLenum coverMode, GLenum transformType, const GLfloat *transformValues);
typedef void (GLAPIENTRYP PFNGLCOVERSTROKEPATHINSTANCEDNVPROC) (GLsizei numPaths, GLenum pathNameType, const GLvoid *paths, GLuint pathBase, GLenum coverMode, GLenum transformType, const GLfloat *transformValues);
typedef void (GLAPIENTRYP PFNGLGETPATHMETRICRANGENVPROC) (GLbitfield metricQueryMask, GLuint firstPathName, GLsizei numPaths, GLsizei stride, GLfloat *metrics);
typedef void (GLAPIENTRYP PFNGLGETPATHSPACINGNVPROC) (GLenum pathListMode, GLsizei numPaths, GLenum pathNameType, const GLvoid *paths, GLuint pathBase, GLfloat advanceScale, GLfloat kerningScale, GLenum transformType, GLfloat *returnedSpacing);
typedef void (GLAPIENTRYP PFNGLPATHPARAMETERINVPROC) (GLuint path, GLenum pname, GLint value);
typedef void (GLAPIENTRYP PFNGLPATHPARAMETERFNVPROC) (GLuint path, GLenum pname, GLfloat value);
typedef void (GLAPIENTRYP PFNGLPATHCOLORGENNVPROC) (GLenum color, GLenum genMode, GLenum colorFormat, const GLfloat *coeffs);
#endif

#define GL_TRANSLATE_2D_NV                                  0x9090

#ifndef __APPLE__
PFNGLGENPATHSNVPROC glGenPathsNV = NULL;
PFNGLDELETEPATHSNVPROC glDeletePathsNV = NULL;
PFNGLPATHCOMMANDSNVPROC glPathCommandsNV = NULL;
PFNGLPATHGLYPHRANGENVPROC glPathGlyphRangeNV = NULL;
PFNGLSTENCILFILLPATHINSTANCEDNVPROC glStencilFillPathInstancedNV = NULL;
PFNGLCOVERFILLPATHINSTANCEDNVPROC glCoverFillPathInstancedNV = NULL;

PFNGLSTENCILSTROKEPATHINSTANCEDNVPROC glStencilStrokePathInstancedNV = NULL;
PFNGLCOVERSTROKEPATHINSTANCEDNVPROC glCoverStrokePathInstancedNV = NULL;

PFNGLGETPATHMETRICRANGENVPROC glGetPathMetricRangeNV = NULL;
PFNGLGETPATHSPACINGNVPROC glGetPathSpacingNV = NULL;

PFNGLPATHPARAMETERINVPROC glPathParameteriNV = NULL;
PFNGLPATHPARAMETERFNVPROC glPathParameterfNV = NULL;
PFNGLPATHCOLORGENNVPROC glPathColorGenNV = NULL;
#endif

#ifndef GL_EXT_framebuffer_sRGB
/* Tokens */
#define GL_FRAMEBUFFER_SRGB_EXT                             0x8DB9
#define GL_FRAMEBUFFER_SRGB_CAPABLE_EXT                     0x8DBA
#endif

#if defined(linux) || defined(sun)
# define GET_PROC_ADDRESS(name)  glXGetProcAddressARB((const GLubyte *) #name)
#elif defined(vxworks)
# define GET_PROC_ADDRESS(name)  rglGetProcAddress(#name)
#elif defined(__APPLE__)
# define GET_PROC_ADDRESS(name)  /*nothing*/
#elif defined(_WIN32)
# define GET_PROC_ADDRESS(name)  wglGetProcAddress(#name)
#else
# error unimplemented code!
#endif

#ifdef __APPLE__
#define LOAD_PROC(type, name)  /*nothing*/
#else
#define LOAD_PROC(type, name) \
	name = (type)GET_PROC_ADDRESS(name);
#endif

void initializeNVPR()
{
	LOAD_PROC(PFNGLGENPATHSNVPROC, glGenPathsNV);
	LOAD_PROC(PFNGLDELETEPATHSNVPROC, glDeletePathsNV);
	LOAD_PROC(PFNGLPATHCOMMANDSNVPROC, glPathCommandsNV);
	LOAD_PROC(PFNGLPATHGLYPHRANGENVPROC, glPathGlyphRangeNV);
	LOAD_PROC(PFNGLSTENCILFILLPATHINSTANCEDNVPROC, glStencilFillPathInstancedNV);
	LOAD_PROC(PFNGLSTENCILSTROKEPATHINSTANCEDNVPROC, glStencilStrokePathInstancedNV);
	LOAD_PROC(PFNGLCOVERFILLPATHINSTANCEDNVPROC, glCoverFillPathInstancedNV);
	LOAD_PROC(PFNGLCOVERSTROKEPATHINSTANCEDNVPROC, glCoverStrokePathInstancedNV);
	LOAD_PROC(PFNGLGETPATHMETRICRANGENVPROC, glGetPathMetricRangeNV);
	LOAD_PROC(PFNGLGETPATHSPACINGNVPROC, glGetPathSpacingNV);
	LOAD_PROC(PFNGLPATHPARAMETERINVPROC, glPathParameteriNV);
	LOAD_PROC(PFNGLPATHPARAMETERFNVPROC, glPathParameterfNV);
	LOAD_PROC(PFNGLPATHCOLORGENNVPROC, glPathColorGenNV);
}