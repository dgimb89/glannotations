#include <glat/RendererFactory.h>
#define NOMINMAX
#include <Windows.h>
#include <gl/GL.h>
#include "config.h"
#include <glat/DistanceFieldRenderer.h>
#ifdef OPTION_USE_NVPR
#include <glat/NVPRFontRenderer.h>
#endif

bool glat::RendererFactory::isExtensionSupported(const char *extension)
{
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

bool glat::RendererFactory::usesNVpr() {
	return m_useNVpr;
}

void glat::RendererFactory::useNVpr(bool useNVpr) {
	m_useNVpr = useNVpr;
}

glat::RendererFactory::RendererFactory() : m_useNVpr(true) {}

glat::AbstractRenderer* glat::RendererFactory::createRenderer() {
#ifdef OPTION_USE_NVPR
	if (usesNVpr() && isExtensionSupported("GL_NV_path_rendering")) {
		return new glat::NVPRFontRenderer();
	}
 else 
#endif
	{
		return new glat::DistanceFieldRenderer();
	}
}