#ifndef RENDERER_H
#define RENDERER_H

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