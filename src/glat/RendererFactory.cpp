#include <glat/RendererFactory.h>
#define NOMINMAX
#include <GL/glew.h>
#include "config.h"
#include <glat/SVGAnnotation.h>
#include <glat/DistanceFieldRenderer.h>
#ifdef OPTION_USE_NVPR
#include <glat/NVPRFontRenderer.h>
#include <glat/NVPRSvgRenderer.h>
#endif

bool glat::RendererFactory::isExtensionSupported(const char *extension) {
	return true;
	const GLubyte *extensions = nullptr;
	const GLubyte *start;
	GLubyte *where, *terminator;

	/* Extension names should not have spaces. */
	where = (GLubyte *)strchr(extension, ' ');
	if (where || *extension == '\0')
		return false;
	extensions = glGetString(GL_EXTENSIONS);

	start = extensions;
	for (;;) {
		where = (GLubyte *)strstr((const char *)start, extension);
		if (!where)
			break;
		terminator = where + strlen(extension);
		if (where == start || *(where - 1) == ' ')
		if (*terminator == ' ' || *terminator == '\0')
			return true;
		start = terminator;
	}
	return false;
}

bool glat::RendererFactory::usesNVpr() const {
	return m_useNVpr && isExtensionSupported("GL_NV_path_rendering");
}

void glat::RendererFactory::useNVpr(bool useNVpr) {
	m_useNVpr = useNVpr;
}

glat::RendererFactory::RendererFactory() : m_useNVpr(true) {}

glat::AbstractRenderer* glat::RendererFactory::createRenderer(const glat::FontAnnotation& annotation) const {
#ifdef OPTION_USE_NVPR
	if (usesNVpr()) {
		return new glat::NVPRFontRenderer();
	}
	else
#endif
	{
		return new glat::DistanceFieldRenderer();
	}
}

glat::AbstractRenderer* glat::RendererFactory::createRenderer(const glat::SVGAnnotation& annotation) const {
#ifdef OPTION_USE_NVPR
	if (usesNVpr()) {
		return new glat::NVPRSvgRenderer();
	}
	else
#endif
	{
		return new glat::DistanceFieldRenderer();
	}
}

glat::AbstractRenderer* glat::RendererFactory::createRenderer(const glat::AbstractAnnotation& annotation) const {
	return nullptr;
}
