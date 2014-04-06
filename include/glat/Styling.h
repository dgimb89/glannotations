#pragma once

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