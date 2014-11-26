#pragma once

#include <string>

#include <glannotations/DirtyFlagObject.h>
#include <glannotations/glannotations_api.h>

namespace glat {
	class GLANNOTATIONS_API Styling : public globjects::Referenced {
	public:
		std::string getID();

	protected:
		Styling();
	};
}