#include <glat/RendererFactory.h>
#include <glat/globals.h>
#include <glat/SVGAnnotation.h>
#include <glat/PNGAnnotation.h>
#include <glat/FontAnnotation.h>
#include <glat/DistanceFieldFontRenderer.h>
#include <glat/DistanceFieldPNGRenderer.h>
#include "glat-version.h"

#include <string.h>

#ifdef OPTION_USE_NVPR
	#include <glat/NVPRFontRenderer.h>
	#include <glat/NVPRSvgRenderer.h>
#endif

bool glat::RendererFactory::isExtensionSupported(const char *extension) {
	return true;
	const gl::GLubyte *extensions = nullptr;
	const gl::GLubyte *start;
	gl::GLubyte *where, *terminator;

	/* Extension names should not have spaces. */
	where = (gl::GLubyte *)strchr(extension, ' ');
	if (where || *extension == '\0')
		return false;
	extensions = gl::glGetString(gl::GL_EXTENSIONS);

	start = extensions;
	for (;;) {
		where = (gl::GLubyte *)strstr((const char *)start, extension);
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

glo::ref_ptr<glat::AbstractRenderer> glat::RendererFactory::createRenderer(const glat::FontAnnotation& annotation) const {
	if (autoInitializesMatricesBuffer()) {
		validateMatricesUBO(m_globalMatricesBindingIndex);
	}
#ifdef OPTION_USE_NVPR
	if (usesNVpr()) {
		return new glat::NVPRFontRenderer(m_globalMatricesBindingIndex);
	}
	else
#endif
	{
		return new glat::DistanceFieldFontRenderer(m_globalMatricesBindingIndex);
	}
}

glo::ref_ptr<glat::AbstractRenderer> glat::RendererFactory::createRenderer(const glat::SVGAnnotation& annotation) const {
	if (autoInitializesMatricesBuffer()) {
		validateMatricesUBO(m_globalMatricesBindingIndex);
	}
#ifdef OPTION_USE_NVPR
	if (usesNVpr()) {
		return new glat::NVPRSvgRenderer(m_globalMatricesBindingIndex);
	}
	else
#endif
	{
		// todo
		throw std::logic_error("The method or operation is not implemented.");
		return nullptr;
	}
}

glo::ref_ptr<glat::AbstractRenderer> glat::RendererFactory::createRenderer(const glat::PNGAnnotation& annotation) const {
	if (autoInitializesMatricesBuffer()) {
		validateMatricesUBO(m_globalMatricesBindingIndex);
	}
	return new glat::DistanceFieldPNGRenderer(m_globalMatricesBindingIndex);
}

void glat::RendererFactory::setAutoInitializeMatricesBuffer(bool autoInitialize) {
	m_autoInitializeMatricesBuffer = autoInitialize;
}

bool glat::RendererFactory::autoInitializesMatricesBuffer() const {
	return m_autoInitializeMatricesBuffer;
}

void glat::RendererFactory::setMatricesBindingIndex(gl::GLuint bindingIndex) {
	m_globalMatricesBindingIndex = bindingIndex;
}

gl::GLuint glat::RendererFactory::getMatricesBindingIndex() const {
	return m_globalMatricesBindingIndex;
}

void glat::RendererFactory::validateMatricesUBO(gl::GLuint bindingIndex) {
	if (!glat::isMatricesUBOInitialiced(bindingIndex)) {
		glat::initializeMatricesUBO(bindingIndex);
	}
}
