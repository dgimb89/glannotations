#include <typeinfo>

#include <glannotations/Styling.h>

std::string glat::Styling::getID() {
	// some magic to demangle class name without name spaces
	return std::string(typeid(*this).name()).substr(20);
}

glat::Styling::Styling() {
}
