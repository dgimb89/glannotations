#include <glat/RenderMethodFactory.h>
#define NOMINMAX
#include <Windows.h>
#include <gl/GL.h>
#include "config.h"
#include <glat/DistanceFieldMapping.h>
#ifdef OPTION_USE_NVPR
#include <glat/NVPathRendering.h>
#endif

bool glat::RenderMethodFactory::isExtensionSupported(const char *extension)
{
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

bool glat::RenderMethodFactory::usesNVpr() {
	return m_useNVpr;
}

void glat::RenderMethodFactory::useNVpr(bool useNVpr) {
	m_useNVpr = useNVpr;
}

glat::RenderMethodFactory::RenderMethodFactory() : m_useNVpr(true) {}

glat::AbstractRenderMethod* glat::RenderMethodFactory::createRenderer() {
#ifdef OPTION_USE_NVPR
	if (usesNVpr() && isExtensionSupported("GL_NV_path_rendering") && isExtensionSupported("GL_EXT_direct_state_access")) {
		return new glat::NVPathRendering();
	}
 else 
#endif
	{
		return new glat::DistanceFieldMapping();
	}
}