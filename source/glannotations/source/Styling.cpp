#include <typeinfo>

#include <glannotations/Styling.h>

std::string glannotations::Styling::getID() {
	// some magic to demangle class name without name spaces
	return std::string(typeid(*this).name()).substr(20);
}

glannotations::Styling::Styling() {
}
