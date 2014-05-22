#pragma once

#include <glat/glat_api.h>

namespace glat {

	// forward declaration 
	class AbstractRenderer;
	class PNGAnnotation;
	class FontAnnotation;
	class SVGAnnotation;

	class GLAT_API RendererFactory {
	public:
		RendererFactory();
		glat::AbstractRenderer* createRenderer(const glat::PNGAnnotation& annotation) const;
		glat::AbstractRenderer* createRenderer(const glat::FontAnnotation& annotation) const;
		glat::AbstractRenderer* createRenderer(const glat::SVGAnnotation& annotation) const;
		void useNVpr(bool useNVpr);
		bool usesNVpr() const;

	protected:
		static bool isExtensionSupported(const char *extension);
		bool m_useNVpr;
	};
}