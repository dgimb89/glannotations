#pragma once

#include <globjects-base/ref_ptr.h>

#include <glat/glat_api.h>

namespace glat {

	// forward declaration 
	class AbstractRenderer;
	class PNGAnnotation;
	class FontAnnotation;
	class SVGAnnotation;

	class GLAT_API RendererFactory {
	public:
		glo::ref_ptr<glat::AbstractRenderer> createRenderer(const glat::PNGAnnotation& annotation) const;
		glo::ref_ptr<glat::AbstractRenderer> createRenderer(const glat::FontAnnotation& annotation) const;
		glo::ref_ptr<glat::AbstractRenderer> createRenderer(const glat::SVGAnnotation& annotation) const;

		void useNVpr(bool useNVpr);
		bool usesNVpr() const;

		void setAutoInitializeMatricesBuffer(bool autoInitialize);
		bool autoInitializesMatricesBuffer() const;
		
		void setMatricesBindingIndex(unsigned int bindingIndex);
		unsigned int getMatricesBindingIndex() const;

	protected:
		static void validateMatricesUBO(unsigned int bindingIndex);
		static bool isExtensionSupported(const char *extension);
		bool m_useNVpr = true;
		bool m_autoInitializeMatricesBuffer = true;
		unsigned int m_globalMatricesBindingIndex = 0;
	};
}