#ifndef GLAT_STYLING_H
#define GLAT_STYLING_H

#include <glat/Object.h>
#include <string>

namespace glat {
	class Styling : public glat::Object {
	public:
		std::string getID();

	protected:
		Styling();
	};
}

#endif