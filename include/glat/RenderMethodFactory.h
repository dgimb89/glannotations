#ifndef RENDER_METHOD_FACTORY_H
#define RENDER_METHOD_FACTORY_H

#include <glat/AbstractRenderMethod.h>

namespace glat {
	class RenderMethodFactory {
	public:
		glat::AbstractRenderMethod* createRenderer();
	protected:
		static bool isExtensionSupported(const char *extension);
	};
}

#endif