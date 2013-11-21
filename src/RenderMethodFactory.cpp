#include <glat/RenderMethodFactory.h>
#include <gl/GL.h>

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

glat::AbstractRenderMethod* glat::RenderMethodFactory::createRenderer() {
#ifdef USE_NVPR
	if (isExtensionSupported("GL_NV_path_rendering") && isExtensionSupported("GL_EXT_direct_state_access")) {

	}
 else 
#endif
	{
	 // distance field
	}
}