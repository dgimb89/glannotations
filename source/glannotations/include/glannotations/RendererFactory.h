#pragma once

#include <globjects/base/ref_ptr.h>

#include <glannotations/glannotations_api.h>

namespace glannotations {

	// forward declaration 
	class AbstractRenderer;
	class PNGAnnotation;
	class FontAnnotation;
	class SVGAnnotation;

	class GLANNOTATIONS_API RendererFactory {
	public:
		globjects::ref_ptr<glannotations::AbstractRenderer> createRenderer(const glannotations::PNGAnnotation& annotation) const;
		globjects::ref_ptr<glannotations::AbstractRenderer> createRenderer(const glannotations::FontAnnotation& annotation) const;
		globjects::ref_ptr<glannotations::AbstractRenderer> createRenderer(const glannotations::SVGAnnotation& annotation) const;

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