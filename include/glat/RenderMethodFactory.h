#ifndef RENDER_METHOD_FACTORY_H
#define RENDER_METHOD_FACTORY_H

namespace glat {

	// we don't care about the interface - forward declaration 
	class AbstractRenderMethod;

	class RenderMethodFactory {
	public:
		RenderMethodFactory();
		glat::AbstractRenderMethod* createRenderer();
		void useNVpr(bool useNVpr);
		bool usesNVpr();

	protected:
		static bool isExtensionSupported(const char *extension);
		bool m_useNVpr;
	};
}

#endif