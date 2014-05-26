#pragma once

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
		RendererFactory();
		glow::ref_ptr<glat::AbstractRenderer> createRenderer(const glat::PNGAnnotation& annotation) const;
		glow::ref_ptr<glat::AbstractRenderer> createRenderer(const glat::FontAnnotation& annotation) const;
		glow::ref_ptr<glat::AbstractRenderer> createRenderer(const glat::SVGAnnotation& annotation) const;
		void useNVpr(bool useNVpr);
		bool usesNVpr() const;

	protected:
		static bool isExtensionSupported(const char *extension);
		bool m_useNVpr;
	};
}