#ifndef GLAT_STYLING_H
#define GLAT_STYLING_H

#include <glat/DirtyFlagObject.h>
#include <string>

namespace glat {
	class Styling : public glow::Referenced {
	public:
		std::string getID();

	protected:
		Styling();
	};
}

#endif