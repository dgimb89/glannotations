#include <typeinfo>

#include <glannotations/Styles/Styling.h>

std::string glannotations::Styling::getID() {
	// some magic to demangle class name without name spaces
	std::string name = std::string(typeid(*this).name());
	return name.substr(name.find_last_of(':') + 1);
}

glannotations::Styling::Styling() {
}
