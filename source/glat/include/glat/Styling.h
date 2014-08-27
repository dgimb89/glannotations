#pragma once

#include <string>

#include <glat/DirtyFlagObject.h>
#include <glat/glat_api.h>

namespace glat {
	class GLAT_API Styling : public glo::Referenced {
	public:
		std::string getID();

	protected:
		Styling();
	};
}