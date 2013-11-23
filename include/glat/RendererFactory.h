#ifndef GLAT_RENDERER_H
#define GLAT_RENDERER_H

namespace glat {

	// forward declaration 
	class AbstractRenderer;

	class RendererFactory {
	public:
		RendererFactory();
		glat::AbstractRenderer* createRenderer();
		void useNVpr(bool useNVpr);
		bool usesNVpr();

	protected:
		static bool isExtensionSupported(const char *extension);
		bool m_useNVpr;
	};
}

#endif