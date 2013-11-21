#ifndef RENDER_METHOD_FACTORY_H
#define RENDER_METHOD_FACTORY_H

#include <glat/AbstractRenderMethod.h>

namespace glat {
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