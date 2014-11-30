#include <glbinding/gl/functions.h>
#include <glbinding/gl/enum.h>

#include <glannotations/RendererFactory.h>
#include <glannotations/globals.h>
#include <glannotations/SVGAnnotation.h>
#include <glannotations/PNGAnnotation.h>
#include <glannotations/FontAnnotation.h>
#include <glannotations/DistanceFieldFontRenderer.h>
#include <glannotations/DistanceFieldPNGRenderer.h>
#include "glannotations-version.h"

#include <string.h>

#ifdef OPTION_USE_NVPR
	#include <glannotations/NVPRFontRenderer.h>
	#include <glannotations/NVPRSvgRenderer.h>
#endif

bool glannotations::RendererFactory::isExtensionSupported(const char *extension) {
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

bool glannotations::RendererFactory::usesNVpr() const {
	return m_useNVpr && isExtensionSupported("GL_NV_path_rendering");
}

void glannotations::RendererFactory::useNVpr(bool useNVpr) {
	m_useNVpr = useNVpr;
}

globjects::ref_ptr<glannotations::AbstractRenderer> glannotations::RendererFactory::createRenderer(const glannotations::FontAnnotation& annotation) const {
	if (autoInitializesMatricesBuffer()) {
		validateMatricesUBO(m_globalMatricesBindingIndex);
	}
#ifdef OPTION_USE_NVPR
	if (usesNVpr()) {
		return new glannotations::NVPRFontRenderer(m_globalMatricesBindingIndex);
	}
	else
#endif
	{
		return new glannotations::DistanceFieldFontRenderer(m_globalMatricesBindingIndex);
	}
}

globjects::ref_ptr<glannotations::AbstractRenderer> glannotations::RendererFactory::createRenderer(const glannotations::SVGAnnotation& annotation) const {
	if (autoInitializesMatricesBuffer()) {
		validateMatricesUBO(m_globalMatricesBindingIndex);
	}
#ifdef OPTION_USE_NVPR
	if (usesNVpr()) {
		return new glannotations::NVPRSvgRenderer(m_globalMatricesBindingIndex);
	}
	else
#endif
	{
		// todo
		throw std::logic_error("The method or operation is not implemented.");
		return nullptr;
	}
}

globjects::ref_ptr<glannotations::AbstractRenderer> glannotations::RendererFactory::createRenderer(const glannotations::PNGAnnotation& annotation) const {
	if (autoInitializesMatricesBuffer()) {
		validateMatricesUBO(m_globalMatricesBindingIndex);
	}
	return new glannotations::DistanceFieldPNGRenderer(m_globalMatricesBindingIndex);
}

void glannotations::RendererFactory::setAutoInitializeMatricesBuffer(bool autoInitialize) {
	m_autoInitializeMatricesBuffer = autoInitialize;
}

bool glannotations::RendererFactory::autoInitializesMatricesBuffer() const {
	return m_autoInitializeMatricesBuffer;
}

void glannotations::RendererFactory::setMatricesBindingIndex(gl::GLuint bindingIndex) {
	m_globalMatricesBindingIndex = bindingIndex;
}

gl::GLuint glannotations::RendererFactory::getMatricesBindingIndex() const {
	return m_globalMatricesBindingIndex;
}

void glannotations::RendererFactory::validateMatricesUBO(gl::GLuint bindingIndex) {
	if (!glannotations::isMatricesUBOInitialiced(bindingIndex)) {
		glannotations::initializeMatricesUBO(bindingIndex);
	}
}
