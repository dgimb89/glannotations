#pragma once

#include <glbinding/types.h>
#include <glowbase/ref_ptr.h>

#include <glat/glat_api.h>

namespace glat {

	// forward declaration 
	class AbstractRenderer;
	class PNGAnnotation;
	class FontAnnotation;
	class SVGAnnotation;

	class GLAT_API RendererFactory {
	public:
		glow::ref_ptr<glat::AbstractRenderer> createRenderer(const glat::PNGAnnotation& annotation) const;
		glow::ref_ptr<glat::AbstractRenderer> createRenderer(const glat::FontAnnotation& annotation) const;
		glow::ref_ptr<glat::AbstractRenderer> createRenderer(const glat::SVGAnnotation& annotation) const;

		void useNVpr(bool useNVpr);
		bool usesNVpr() const;

		void setAutoInitializeMatricesBuffer(bool autoInitialize);
		bool autoInitializesMatricesBuffer() const;
		
		void setMatricesBindingIndex(gl::GLuint bindingIndex);
		gl::GLuint getMatricesBindingIndex() const;

	protected:
		static void validateMatricesUBO(gl::GLuint bindingIndex);
		static bool isExtensionSupported(const char *extension);
		bool m_useNVpr = true;
		bool m_autoInitializeMatricesBuffer = true;
		gl::GLuint m_globalMatricesBindingIndex = 0;
	};
}